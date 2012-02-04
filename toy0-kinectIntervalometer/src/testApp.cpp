#include "testApp.h"

testApp::testApp() :
kinectView(kinect),

scrPreviewDepth("Depth", kinect.getDepthTextureReference()),
scrPreviewRGB("RGB", kinect.getRGBTextureReference()),
scr3D("3D", kinectView),

wdgSelectPath("Select path")

{
	scrMain.push(scrPreviewDepth);
	scrMain.push(scrPreviewRGB);
	scrMain.push(scrControl);
	scrMain.push(scr3D);
	
	scr3D.enableGrid(3.0f);
	scrControl.push(new wdgButton("recording", recording));
	scrControl.push(new wdgSlider("interval", interval, 0, 20, 0.1, "s"));
	scrControl.push(new wdgCounter("snaps taken", count));
	scrControl.push(&wdgSelectPath);
}

//--------------------------------------------------------------
void testApp::setup(){	
	screens.init(scrMain);	
	ofBackground(117/2,130/2,160/2);
	ofSetVerticalSync(true);
	kinect.setupFromXML("openni/config/ofxopenni_config.xml",false);
	kinect.enableCalibratedRGBDepth();

	lastCapture = 0;
	count = 0;
	interval = 1.0f;
	recording = true;
	path = "";
	
	startThread(true, false);
}

//--------------------------------------------------------------
void testApp::update(){
	
	if (wdgSelectPath.getBang())
		path = ofSystemSaveDialog("timelapse", "Timelapse save path").getPath();
	
	lock();
	kinect.update();
	unlock();
}

void testApp::threadedFunction() {
	while (isThreadRunning()) {
		if (recording && ofGetElapsedTimef() - lastCapture >= interval)
			if (kinect.isNewFrame())
				capture();
		ofSleepMillis(10);
	}
}
//--------------------------------------------------------------
void testApp::draw(){
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::capture(){ 
	stringstream dateString;
	dateString << ofGetYear() << "-" << ofGetMonth() << "-" << ofGetDay() << " " << ofGetHours() << "." << ofGetMinutes() << "." << ofGetSeconds();
	
	lock();
	rgb = kinect.getRGBPixels();
	depth = kinect.getDepthRawPixels();
	unlock();
	
	ofSaveImage(rgb, path + "/" + dateString.str() + string("-rgb.jpg"));
	ofSaveImage(depth, path + "/" + dateString.str() + string("-depth.png"));
	
	lastCapture = ofGetElapsedTimef();
	count++;
}