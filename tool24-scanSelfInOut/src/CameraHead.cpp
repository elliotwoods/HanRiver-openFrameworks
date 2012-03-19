#include "CameraHead.h"

//-----------
CameraHead::CameraHead() {
	this->payload = 0;
	this->setProjectorID(0);
	this->initialised = false;
}

//-----------
CameraHead::CameraHead(int cameraID, const Payload& payload) {
	this->init(cameraID, payload);
}

//-----------
CameraHead::~CameraHead() {
	this->camera.close();
}

//-----------
void CameraHead::init(int cameraID, const Payload& payload) {
	this->initialised = camera.init(cameraID, true, IS_SET_CM_BAYER);
	ofxUeyePreset_5480SL().apply(this->camera);
	this->payload = &payload;
	this->setProjectorID(0);
}

//-----------
void CameraHead::update() {
	this->getDecoder().update();
	this->camera.update();
}

//-----------
int CameraHead::getCameraID() {
	return this->camera.getCameraID();
}

//-----------
void CameraHead::setProjectorID(int projector) {
	if (!this->initialised)
		return;

	if (decoders.count(this->projector) > 0) {
		this->decoders[this->projector].clearCaptures();
	}

	this->projector = projector;
	if (decoders.count(projector) == 0) {
		decoders.insert(pair<int, Decoder>(projector, Decoder()));
		decoders[projector].init(*payload);
	}
}

//-----------
int CameraHead::getProjectorID() const {
	return this->projector;
}

//-----------
ofxUeye& CameraHead::getCamera() {
	return (this->camera);
}

//-----------
Decoder& CameraHead::getDecoder() {
	return this->decoders[projector];
}

//-----------
bool CameraHead::capture() {
	if (!this->initialised)
		return false;

	ofLogNotice() << "Camera " << this->getCameraID() << " capturing frame " << this->getDecoder().getFrame();
	ofSleepMillis(40); //delay for projector
	while (! this->camera.capture())
		ofSleepMillis(10);
	while (! this->camera.capture())
		ofSleepMillis(10);
	
	this->getDecoder() << camera.getPixelsRef();
	return this->getDecoder().hasData();
}

//-----------
void CameraHead::clear() {
	if (!this->initialised)
		return;

#pragma omp critical(ofLog)
	ofLogNotice() << "Camera " << this->getCameraID() << " resetting decoder";
	this->getDecoder().reset();
}

//-----------
void CameraHead::moveThreshold(int distance) {
	int threshold = this->getDecoder().getThreshold() + distance;

#pragma omp critical(ofLog)
	ofLogNotice() << "Setting threshold for Projector " << this->getProjectorID() << " in Camera " << this->getCameraID() << " at " << threshold;

	this->getDecoder().setThreshold(ofClamp(threshold, 0, 255));
}

//-----------
void CameraHead::load() {
	map<int, Decoder>::iterator it;
	for (it = decoders.begin(); it != decoders.end(); it++)
		it->second.loadDataSet("c" + ofToString(this->getCameraID()) + "-p" + ofToString(this->projector) + ".DataSet");
}

//-----------
void CameraHead::save() {
	map<int, Decoder>::iterator it;
	for (it = decoders.begin(); it != decoders.end(); it++) {
#pragma omp critical(stringerror)
		it->second.saveDataSet("c" + ofToString(this->getCameraID()) + "-p" + ofToString(it->first) + ".DataSet");
		//it->second.getDataSet().saveCorrespondences("c" + ofToString(this->getCameraID()) + "-p" + ofToString(it->first) + ".correspondences");
	}
}

//-----------
void CameraHead::saveCurrent() {
	this->getDecoder().saveDataSet("c" + ofToString( this->getCameraID() ) + "-p" + ofToString( this->getProjectorID() ) + ".DataSet");
}