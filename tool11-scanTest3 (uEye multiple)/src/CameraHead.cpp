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
	camera.update(); //set fresh to false
	camera.update(); //read new isFrameNew
	while (!camera.isFrameNew()) {
		camera.update();
		ofSleepMillis(1);
	}

	ofPixels buffer;
	camera.copyPixelsTo(buffer); //this function locks the thread during copy
	decoder << buffer;
}

//-----------
void CameraHead::clear() {
	decoder.reset();
}

