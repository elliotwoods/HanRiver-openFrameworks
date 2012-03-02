#include "CameraHead.h"
//------------
CameraHead::CameraHead(const ofxUeyeDevice& device) {
	//camera
	if (!this->camera.init(device, IS_CM_MONO8)) {
		ofLogError() << "Cannot initialise camera " << device.toString();
		system("pause");
		std::exit(0);
	}
	ofxUeyePreset_5480Chessboard().apply(this->camera);
	
	//downSampled
	float downsampledWidth = this->camera.getWidth();
	float downsampledHeight = this->camera.getHeight();
	while (downsampledWidth > MAX_DOWNSAMPLED_RES || downsampledHeight > MAX_DOWNSAMPLED_RES) {
		downsampledWidth *= 0.75f;
		downsampledHeight *= 0.75f;
	}
	this->downSampled.allocate(downsampledWidth, downsampledHeight, OF_PIXELS_MONO);
	this->needsUpdate = false;
	this->preview.allocate(downSampled);

	//calibration
	this->found = false;

	//thread
	this->startThread(true, false);
}

//------------
CameraHead::~CameraHead() {
	this->stopThread(false);
}

//------------
void CameraHead::add() {
	lock();
	camera.capture();
	makePreview(false);
	captures.add(camera.getPixelsRef(), this->downSampled);
	

	unlock();
}

//------------
const ofPixels& CameraHead::getPixelsRef() const {
	return this->downSampled;
}

//------------
const ofxUeye& CameraHead::getCamera() const {
	return this->camera;
}

//------------
const vector<ofPixels>& CameraHead::getCaptures() const {
	return this->captures.getPreviews();
}

//------------
void CameraHead::draw(float x, float y) {
	this->draw(x, y, this->getWidth(), this->getHeight());
}

//------------
void CameraHead::draw(float x, float y, float width, float height) {
	if (this->needsUpdate) {
		this->lock();
		this->preview.loadData(downSampled);
		this->unlock();
	}
	preview.draw(x, y, width, height);

	if (found) {
		vector<Point2f>::iterator it;
		ofVec2f cornerInView;
		ofBeginShape();
		ofPushStyle();
		ofNoFill();
		ofSetColor(200,100,100);
		ofSetLineWidth(2);
		for (it = previewCorners.begin(); it != previewCorners.end(); it++) {
			//hopefully compiler will optimise this!
			cornerInView = ofVec2f(it->x, it->y) / ofVec2f(preview.getWidth(), preview.getHeight()) * ofVec2f(width, height) + ofVec2f(x, y);
			ofCircle(cornerInView, 10);
			ofVertex((ofVec3f)cornerInView);
		}
		ofSetColor(255,255,255);
		ofSetLineWidth(1);
		ofEndShape(false);
		ofPopStyle();
	}

	AssetRegister.drawText(string("Reprojection error : ") + ofToString(captures.getReprojectionError()), 20, 60, "", true, 30);
}

//------------
float CameraHead::getWidth() {
	return preview.getWidth();
}

//------------
float CameraHead::getHeight() {
	return preview.getHeight();
}

//------------
void CameraHead::threadedFunction() {
	while (this->isThreadRunning()) {
		capture();
		makePreview(true);
	}
}

//------------
void CameraHead::capture() {
	this->lock();
	this->camera.capture();
	this->unlock();
}

//------------
void CameraHead::makePreview(bool lock) {
	//lots of thread locks/unlocks to allow add() to interrupt
	if (lock)
		this->lock();
	this->camera.getPixelsRef().resizeTo(downSampled);
	ofPixels myPixels = downSampled;
	if (lock)
		this->unlock();
	this->needsUpdate = true;

	if (myPixels.isAllocated()) {
		if(ofxCv::findChessboardCorners(toCv(myPixels), cv::Size(PATTERN_SIZE), previewCorners, CV_CALIB_CB_NORMALIZE_IMAGE | CV_CALIB_CB_FAST_CHECK)) {
			found = ofxCv::findChessboardCorners(toCv(myPixels), cv::Size(PATTERN_SIZE), previewCorners, CV_CALIB_CB_ADAPTIVE_THRESH);
		} else
			found = false;
	}
}
