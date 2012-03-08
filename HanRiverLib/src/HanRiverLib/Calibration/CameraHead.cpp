#include "CameraHead.h"

namespace HanRiverLib {
	//----------
	void CameraHead::init(const ofxUeyeDevice & device) {
		this->cameraID = device.cameraID;
		this->camera.init(device);
		this->hasIntrinsics = false;
		this->newFrame = false;
	}

	//----------
	void CameraHead::capture(int captureID) {
		if (camera.capture()) {
			boards.insert(std::pair<int, ofPtr<BoardFrame> >(captureID, new BoardFrame(camera.getPixelsRef()) ) );
			newFrame = true;
		}
	}

	//----------
	void CameraHead::solveIntrinsics() {
		//wait for all boards to be complete
		unsigned int waitingFor = 1;
		map<int, ofPtr<BoardFrame> >::const_iterator boardsIt;
		while (waitingFor > 0) {
			for (boardsIt = boards.begin(); boardsIt != boards.end(); boardsIt++) {
				waitingFor = 0;
				if (!boardsIt->second->isComplete())
					waitingFor++;
			}
	#pragma omp critical(ofLog)
			ofLogNotice("CameraHead") << "Camera " << cameraID << " still waiting to find " << waitingFor << " boards out of " << boards.size();
		}

		//find which boards have imagepoints
		successfulFinds.clear();
		for (boardsIt = boards.begin(); boardsIt != boards.end(); boardsIt++)
			if (boardsIt->second->isSuccess())
				successfulFinds.insert(boardsIt->first);


		//create vector<vector<Point2f>> with successes
		vector<vector<Point2f> > imagePoints = this->getImagePoints(successfulFinds);

		//calibrate
		this->calibrateIntrinsics(imagePoints);
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

		ofPushStyle();
		ofSetLineWidth(2.0f);
		ofColor recent(200, 100, 100);
		ofColor stale(100, 100, 100);
		ofColor current;
		ofPoint posInView;
		for (int i=0; i<boards.size(); i++) {
			if (boards[i]->isSuccess()) {
				current = stale;
				current.lerp(recent, (float)(i + 1.0f) / (float)boards.size());
				ofSetColor(current);

				ofBeginShape();
				const vector<Point2f> & imagePoints(boards[i]->getImagePoints());
				for (int j=0; j<imagePoints.size(); j++) {
					posInView.x = imagePoints[j].x / this->getWidth() * w + x;
					posInView.y = imagePoints[j].y / this->getHeight() * h + y;
					ofVertex(posInView);
					ofCircle(posInView, 10);
				}
				ofEndShape();
				ofDrawBitmapString(ofToString(i), posInView);
			}
		}
		ofPopStyle();
	}

	//----------
	float CameraHead::getWidth() {
		return this->camera.getWidth();
	}

	//----------
	float CameraHead::getHeight() {
		return this->camera.getHeight();
	}

	//----------
	void CameraHead::calibrateIntrinsics(const vector<vector<Point2f> > & imagePoints) {
		if (imagePoints.size() < 3) {
			ofLogError("CameraHead") << "We have insufficient captures (" << imagePoints.size() << ") to calculate intrinsics for camera " << this->cameraID;
			this->hasIntrinsics = false;
			return;
		}

		ofxCv::Calibration calibration = boardFinder;
		calibration.imagePoints = imagePoints;
		calibration.setImageSize(cv::Size(this->camera.getWidth(), this->camera.getHeight()));

		this->reprojectionError = calibration.calibrate();
		this->intrinsics = calibration.getDistortedIntrinsics();
		this->distortion = calibration.getDistCoeffs();
		this->hasIntrinsics = true;
	}
}