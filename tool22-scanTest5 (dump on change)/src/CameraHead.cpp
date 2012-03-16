#include "CameraHead.h"

//-----------
CameraHead::CameraHead() {
	this->payload = 0;
	this->setProjectorID(0);
	this->gui = 0;
	this->initialised = false;
	this->projector = 0;
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
	this->panelPinC = gui.add(this->getDecoder().getProjectorInCamera(), "Projector in Camera " + ofToString( this->getCameraID() ));
	this->panelCinP = gui.add(this->getDecoder().getCameraInProjector(), "Camera " + ofToString( this->getCameraID() ) + " in Projector");
	gui.add( this->getDecoder().getCaptures() );
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

	if (this->projector != projector)
		this->save();

	this->decoder.reset();
	this->decoder.init(*payload);
	this->projector = projector;

	if ( this->panelPinC != PanelPtr() )
		panelPinC->setCaption( "P " + ofToString(this->projector) + " in C " + ofToString( this->getCameraID() ) );
	if ( this->panelCinP != PanelPtr() )
		panelCinP->setCaption( "C " + ofToString(this->getCameraID()) + " in P " + ofToString( this->projector ) );
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
	return this->decoder;
}

//-----------
void CameraHead::capture() {
	if (!this->initialised)
		return;

#pragma omp critical(ofLog)
	ofLogNotice() << "Camera " << this->getCameraID() << " capturing frame " << this->getDecoder().getFrame();
	ofSleepMillis(20); //delay for projector
	camera.capture();
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
void CameraHead::save() {
	decoder.saveDataSet("c" + ofToString(this->getCameraID()) + "-p" + ofToString(this->projector) + ".DataSet");
	decoder.getDataSet().saveCorrespondences("c" + ofToString(this->getCameraID()) + "-p" + ofToString(this->projector) + ".correspondences");
}

