#include "CameraHead.h"

//-----------
CameraHead::CameraHead() {
	this->payload = 0;
}

//-----------
CameraHead::CameraHead(int cameraID, const Payload& payload) {
	this->init(cameraID, payload);
}

//-----------
void CameraHead::init(int cameraID, const Payload& payload) {
	this->camera = ofPtr<ofxUeyeThreaded>(new ofxUeyeThreaded());
	camera->init(cameraID, true, IS_SET_CM_BAYER);
	decoder.init(payload);
	this->payload = &payload;
}

//-----------
void CameraHead::update() {
	this->camera->update();
}

//-----------
int CameraHead::getID() {
	return this->camera->getCamera().getCameraID();;
}

//-----------
ofxUeyeThreaded& CameraHead::getCamera() {
	return *(this->camera);
}

//-----------
Decoder& CameraHead::getDecoder() {
	return this->decoder;
}
