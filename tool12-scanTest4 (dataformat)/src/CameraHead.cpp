#include "CameraHead.h"

//-----------
CameraHead::CameraHead() {
	this->payload = 0;
	this->setProjectorID(0);
	this->gui = 0;
	this->initialised = false;
}

//-----------
CameraHead::~CameraHead() {
	this->camera.close();
}

//-----------
CameraHead::CameraHead(int cameraID, const Payload& payload, Builder& gui) {
	this->init(cameraID, payload, gui);
	this->setProjectorID(0);
}

//-----------
CameraHead::CameraHead(int cameraID, const Payload& payload) {
	this->init(cameraID, payload);
	this->setProjectorID(0);
	this->gui = 0;
}

//-----------
void CameraHead::init(int cameraID, const Payload& payload, Builder& gui) {
	this->init(cameraID, payload);
	this->gui = &gui;
	gui.add(this->camera, "Camera " + ofToString(this->getCameraID()));
}


//-----------
void CameraHead::init(int cameraID, const Payload& payload) {
	this->initialised = camera.init(cameraID, true, IS_SET_CM_BAYER);
	this->camera.apply(ofxUeyePreset_5480SL());
	this->payload = &payload;
}

//-----------
void CameraHead::update() {
	this->getDecoder().update();
	this->camera.update();
}

//-----------
int CameraHead::getCameraID() {
	return this->camera.getCamera().getCameraID();
}

//-----------
void CameraHead::setProjectorID(int projector) {
	if (!this->initialised)
		return;

	if (decoders.count(this->projector) > 0)
		this->decoders[this->projector].clearCaptures();

	this->projector = projector;
	if (decoders.count(projector) == 0) {
		decoders.insert(pair<int, Decoder>(projector, Decoder()));
		decoders[projector].init(*payload);

		if (gui != 0) {
			gui->drop(captureSetPanel);
			captureSetPanel = gui->add(this->getDecoder().getCaptures(), "Camera " + ofToString(this->getCameraID()) + " capture set for Projector " + ofToString(projector));
			gui->add(this->getDecoder().getProjectorInCamera(), "Projector " + ofToString(projector) + " in Camera " + ofToString(this->getCameraID()));
			gui->add(this->getDecoder().getCameraInProjector(), "Camera " + ofToString(this->getCameraID()) + " in Projector " + ofToString(projector));
		}
	}
}

//-----------
int CameraHead::getProjectorID() const {
	return this->projector;
}

//-----------
ofxUeyeThreaded& CameraHead::getCamera() {
	return (this->camera);
}

//-----------
Decoder& CameraHead::getDecoder() {
	return this->decoders[projector];
}

//-----------
void CameraHead::capture() {
	if (!this->initialised)
		return;

#pragma omp critical(ofLog)
	ofLogNotice() << "Camera " << this->getCameraID() << " capturing frame " << this->getDecoder().getFrame();
	ofSleepMillis(20); //delay for projector
	this->getDecoder() << camera.getFreshFrameCopy(); //take copy
	this->camera.setThreadPaused(this->getDecoder().getFrame() != 0 && this->getDecoder().getFrame() < this->getDecoder().getFrameCount());
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
		it->second.saveDataSet("c" + ofToString(this->getCameraID()) + "-p" + ofToString(it->first) + ".DataSet");
		it->second.getDataSet().saveCorrespondences("c" + ofToString(this->getCameraID()) + "-p" + ofToString(it->first) + ".correspondences");
	}
}

