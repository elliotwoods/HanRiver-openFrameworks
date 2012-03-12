#include "CameraHead.h"

//-----------
CameraHead::CameraHead() {
	this->payload = 0;
}

//-----------
CameraHead::~CameraHead() {
	this->camera.close();
}

//-----------
CameraHead::CameraHead(int cameraID, const Payload& payload) {
	this->init(cameraID, payload);
}

//-----------
void CameraHead::init(int cameraID, const Payload& payload) {
	camera.init(cameraID, true, IS_SET_CM_BAYER);
	this->camera.apply(ofxUeyePreset_5480SL());
	decoder.init(payload);
	this->payload = &payload;
}

//-----------
void CameraHead::update() {
	this->camera.update();
}

//-----------
int CameraHead::getID() {
	return this->camera.getCamera().getCameraID();
}

//-----------
ofxUeyeThreaded& CameraHead::getCamera() {
	return (this->camera);
}

//-----------
Decoder& CameraHead::getDecoder() {
	return this->decoder;
}

//-----------
void CameraHead::capture() {
#pragma omp critical(ofLog)
	ofLogNotice() << "Camera " << this->getID() << " capturing frame " << decoder.getFrame();
	ofSleepMillis(10); //delay for projector
	decoder << camera.getFreshFrameCopy(); //take copy
}

//-----------
void CameraHead::clear() {
#pragma omp critical(ofLog)
	ofLogNotice() << "Camera " << this->getID() << " resetting decoder";
	decoder.reset();
}

