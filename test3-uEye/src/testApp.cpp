#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofAddListener(ueye.events.dimensionChanged, this, &testApp::ueyeDimensionChanged);

	ueye.setVerbose(true);
	ueye.listDevices();

	if(ueye.init())
	{
		//cout << (IS_BINNING_8X_VERTICAL | IS_BINNING_8X_HORIZONTAL) << endl;
		// Get full area of the sensor, but skipping every second pixel
		ueye.setBinning(IS_BINNING_2X_VERTICAL | IS_BINNING_2X_HORIZONTAL); // difference from subsamplimg? (apparently same bandwith but smoother image)
	
		// smooth the bad pixels (apparently they come from factory with bad pixels...)
		//ueye.enableBadPixelsCorrection();
	
		// Set AOI (always set AOI after binning, subsampling or scaler, otherwise you might not get the desired result)
		ofRectangle fullHD;
		fullHD.width = 640;
		fullHD.height = 480;
		fullHD.x = 0;
		fullHD.y = (ueye.getAOIMax().height - fullHD.height) * 0.5;
		ueye.setAOI(fullHD);
		//ueye.setAOINormalized(ofRectangle(0,0, 0.6, 0.6));
			
		// Start grabbing pixels
		ueye.enableLive();

		settings.setup(&ueye);
	}

	
}
//--------------------------------------------------------------
void testApp::ueyeDimensionChanged(ofxUeyeEventArgs &args){
	// If we got here, bandwith has changed.
	// Pixel Clock, FPS and Exposure should be adjusted.
	//ueye.setPixelClock(ueye.getPixelClockMax());
	//ueye.setFPS(ueye.getFPSMax());
	//ueye.setFPS(60);

	tex.clear();
	tex.allocate(ueye.getWidth(), ueye.getHeight(),GL_RGB);
}
//--------------------------------------------------------------
void testApp::update(){
	ueye.update();
	if(ueye.isReady() && ueye.isFrameNew())
		tex.loadData(ueye.getPixels(), ueye.getWidth(), ueye.getHeight(), GL_RGB);
}
//--------------------------------------------------------------
void testApp::draw(){
	if(ueye.isReady())
		tex.draw((ofGetWidth()-tex.getWidth())/2, (ofGetHeight()-tex.getHeight())/2);	
	settings.draw(20,20);
}
//--------------------------------------------------------------
void testApp::exit(){
	ueye.close();
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	settings.keyPressed(key);
}


