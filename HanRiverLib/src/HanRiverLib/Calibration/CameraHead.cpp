#include "CameraHead.h"

namespace HanRiverLib {
	//----------
	CameraHead::CameraHead() {
		this->hasDevice = false;
		this->hasIntrinsics = false;
	}

	//----------
	CameraHead::~CameraHead() {
		this->close();
	}

	//----------
	void CameraHead::init(uint16_t cameraID, uint16_t width, uint16_t height) {
		this->cameraID = cameraID;
		this->width = width;
		this->height = height;
	}

	//----------
	void CameraHead::init(const ofxUeyeDevice & device) {
		this->cameraID = device.cameraID;
		this->camera.init(device);
		ofxUeyePreset_5480Chessboard().apply(this->camera);
		this->hasDevice = true;
		this->newFrame = false;
		this->width = this->camera.getWidth();
		this->height = this->camera.getHeight();
	}

	//----------
	void CameraHead::close() {
		this->camera.close();
	}

	//----------
	void CameraHead::capture(int captureID) {
		if (this->hasDevice) {
			if (camera.capture()) {
				boards.insert(std::pair<int, ofPtr<BoardFrame> >(captureID, new BoardFrame(camera.getPixelsRef()) ) );
				newFrame = true;
			}
		}
	}

	//----------
	void CameraHead::solveIntrinsics() {
		this->waitForComplete();
		this->updateSuccessfulFinds();


		//create vector<vector<Point2f>> with successes
		vector<vector<Point2f> > imagePoints = this->getImagePoints(successfulFinds);

		//calibrate
		this->calibrateIntrinsics(imagePoints);
	}

	//----------
	void CameraHead::setExtrinsics(const ofMatrix4x4 & transform) {
		this->extrinsics = transform;
	}

	//----------
	vector<vector<Point2f> > CameraHead::getImagePoints(const set<int> & captureIDs) const {
		vector<vector<Point2f> > imagePoints;
		set<int>::const_iterator it;
		for (it = captureIDs.begin(); it != captureIDs.end(); it++) {
			if (this->successfulFinds.count(*it) != 0) {
				BoardFrame & frame (*this->boards.at(*it));
				imagePoints.push_back(frame.getImagePoints());
			}
		}
		return imagePoints;
	}

	//----------
	bool CameraHead::getHasIntrinsics() const {
		return this->hasIntrinsics;
	}

	//----------
	int CameraHead::getCameraID() const{
		return this->cameraID;
	}

	//----------
	const set<int> & CameraHead::getSuccessfulFinds() const {
		return this->successfulFinds;
	}

	//----------
	set<int> CameraHead::getCommonSuccessfulFinds(const set<int> & otherSuccessfulFinds) const {
		set<int> common;
		set<int>::iterator it;
		for (it = otherSuccessfulFinds.begin(); it != otherSuccessfulFinds.end(); it++)
			if (this->successfulFinds.count(*it) != 0)
				common.insert(*it);
		return common;
	}

	//----------
	set<int> CameraHead::getCommonSuccessfulFinds(const CameraHead & other) const {
		return this->getCommonSuccessfulFinds(other.getSuccessfulFinds());
	}

	//----------
	void CameraHead::getCalibration(Mat & cameraMatrix, Mat & distortion) const {
		cameraMatrix = this->intrinsics.getCameraMatrix();
		distortion = this->distortion;
	}

	//----------
	const ofMatrix4x4 & CameraHead::getFirstBoardTransform() const {
		return this->firstBoardTransform;
	}

	//----------
	void CameraHead::draw(float x, float y) {
		this->draw(x, y, this->getWidth(), this->getHeight());
	}

	//----------
	void CameraHead::draw(float x, float y, float w, float h) {
		if (!preview.isAllocated())
			preview.allocate(camera.getPixelsRef());
		if (newFrame)
			preview.loadData(camera.getPixelsRef());

		preview.draw(x,y,w,h);

		string message = "";
		ofPushStyle();
		ofEnableSmoothing();
		ofSetLineWidth(1.0f);
		ofNoFill();
		ofColor current, base(200, 100, 100);
		ofPoint posInView;
		set<int>::iterator it;
		for (it = successfulFinds.begin(); it != successfulFinds.end(); it++) {
			if (message != "")
				message += ", ";
			message += ofToString(*it);
			current = base;
			current.setHue( ofMap(*it, 0, 30, 0, 120) );
			ofSetColor(current);

			ofBeginShape();
			const vector<Point2f> & imagePoints(boards[*it]->getImagePoints());
			for (int j=0; j<imagePoints.size(); j++) {
				posInView.x = imagePoints[j].x / this->getWidth() * w + x;
				posInView.y = imagePoints[j].y / this->getHeight() * h + y;
				ofVertex(posInView);
				ofCircle(posInView, 5);
			}
			ofEndShape();
			ofSetColor(255);
			ofDrawBitmapString(ofToString(*it), posInView);
		}
		ofPopStyle();

		ofxCvGui::AssetRegister.drawText(message, 20, 60, "swisop2:10", true);
		if (!this->hasDevice)
		ofxCvGui::AssetRegister.drawText("No camera device available", 20, 90, "swisop2:10", true);
	}

	//----------
	float CameraHead::getWidth() {
		return (float)this->width;
	}

	//----------
	float CameraHead::getHeight() {
		return (float)this->height;
	}

	//----------
	float CameraHead::getWidth() const {
		return (float)this->width;
	}

	//----------
	float CameraHead::getHeight() const {
		return (float)this->height;
	}

	//----------
	cv::Size CameraHead::getImageSize() const {
		return cv::Size(this->width, this->height);
	}

	//----------
	void CameraHead::load() {
		//look for files which define the 
		ifstream file;
		file.open(ofToDataPath(this->getFilenameBase()), ios::binary);

		if (!file.is_open() || file.bad()) {
#pragma omp critical(ofLog)
			ofLogNotice("CameraHead") << "Camera " << cameraID << " cannot load";
			file.close();
			return;
		}

		this->successfulFinds.clear();
		this->boards.clear();
		uint16_t count;
		int find;
		file.read((char*)&count, sizeof(count));
		for (int i=0; i<count; i++) {
			file.read((char*)&find, sizeof(find));
			this->successfulFinds.insert(find);
		}

		set<int>::const_iterator it;
		for (it = this->successfulFinds.begin(); it != this->successfulFinds.end(); it++) {
			this->boards.insert( pair<int, ofPtr<BoardFrame> > (
				*it,
				new BoardFrame( this->getBoardFilename(*it) ) ) );
		}

		file.close();
	}

	//----------
	void CameraHead::save() {
		this->waitForComplete();
		this->updateSuccessfulFinds();

#pragma omp critical(ofLog)
		ofLogNotice("CameraHead") << "Saving image points for Camera " << this->cameraID;

		ofstream file;
		file.open(ofToDataPath(this->getFilenameBase()), ios::binary);

		if (!file.is_open() || file.bad()) {
#pragma omp critical(ofLog)
			ofLogNotice("CameraHead") << "Camera " << this->cameraID << " cannot save";
			file.close();
			return;
		}

		uint16_t count = this->successfulFinds.size();
		int find;
		file.write((char*)&count, sizeof(count));
		set<int>::const_iterator it;
		for (it = this->successfulFinds.begin(); it != this->successfulFinds.end(); it++) {
			file.write((char*)&*it, sizeof(*it));
			boards.at(*it)->save( this->getBoardFilename(*it) );
		}

		file.close();
	}
	
	//----------
	const Intrinsics & CameraHead::getIntrinsics() const {
		return this->intrinsics;
	}
	
	//----------
	const Mat & CameraHead::getDistortion() const {
		return this->distortion;
	}

	//----------
	const ofMatrix4x4 & CameraHead::getExtrinsics() const {
		return this->extrinsics;
	}

	//----------
	void CameraHead::waitForComplete() const {
		//wait for all boards to be complete
		unsigned int waitingFor = 1;
		map<int, ofPtr<BoardFrame> >::const_iterator boardsIt;
		while (waitingFor > 0) {
			for (boardsIt = boards.begin(); boardsIt != boards.end(); boardsIt++) {
				waitingFor = 0;
				if (!boardsIt->second->isComplete())
					waitingFor++;
			}
			if (waitingFor > 0) {
		#pragma omp critical(ofLog)
				ofLogNotice("CameraHead") << "Camera " << cameraID << " still waiting to find boards (" << waitingFor << "  left out of " << boards.size() << ")";
				ofSleepMillis(3000);
			}
		}
	}

	//----------
	void CameraHead::updateSuccessfulFinds() {
		successfulFinds.clear();
		map<int, ofPtr<BoardFrame> >::const_iterator boardsIt;
		for (boardsIt = boards.begin(); boardsIt != boards.end(); boardsIt++)
			if (boardsIt->second->isSuccess())
				successfulFinds.insert(boardsIt->first);
	}

	//----------
	void CameraHead::calibrateIntrinsics(const vector<vector<Point2f> > & imagePoints) {
		if (imagePoints.size() < 3) {
	#pragma omp critical(ofLog)
			ofLogError("CameraHead") << "We have insufficient captures (" << imagePoints.size() << ") to calculate intrinsics for camera " << this->cameraID;
			this->hasIntrinsics = false;
			return;
		}

		ofxCv::Calibration calibration = GlobalBoardFinder::boardFinder;
		calibration.imagePoints = imagePoints;
		calibration.setImageSize(cv::Size(this->getWidth(), this->getHeight()));

		this->reprojectionError = calibration.calibrate();
		this->intrinsics = calibration.getDistortedIntrinsics();
		this->distortion = calibration.getDistCoeffs();
		this->hasIntrinsics = true;
		this->firstBoardTransform = calibration.getBoardTransformation(0);
#pragma omp critical(ofLog)
		{
			ofLogNotice("CameraHead") << "Camera " << this->getCameraID() << " calibrated." << endl <<
			"....Camera matrix:" << endl << this->intrinsics.getCameraMatrix() << endl <<
			"....Distortion " << endl << this->distortion << endl <<
			"....FOV: " << this->intrinsics.getFov() << endl <<
			"....Aspect ratio: " << this->intrinsics.getAspectRatio() << endl << endl;
		}
	}

	//----------
	string CameraHead::getFilenameBase() const {
		return string("c") + ofToString(this->getCameraID());
	}

	//----------
	string CameraHead::getBoardFilename(int board) const {
		return this->getFilenameBase() + "b" + ofToString(board);
	}
}
