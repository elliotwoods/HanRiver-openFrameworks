#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	load();
}

//--------------------------------------------------------------
void testApp::load() {
	string path = ofSystemLoadDialog("Select folder with images", true).getPath();
	cout << path;

	ofDirectory imageSet;
	int fileCount = imageSet.listDir(path);

	if (fileCount==0) {
		ofLogError() << "no images found";
		return;
	}

	int width, height;
	int entriesPerWidth;
	ofImage first(imageSet.getPath(0));
	width = first.getWidth();
	height = first.getHeight();
	const int channels = first.getPixelsRef().getNumChannels();

	int totalWidth = 1024;
	entriesPerWidth = totalWidth / width;
	int totalHeight = ceil((float)fileCount / (float)entriesPerWidth) * height;
	assembled.allocate(totalWidth, totalHeight, channels == 4 ? OF_IMAGE_COLOR_ALPHA : OF_IMAGE_COLOR);
	
	vector<ofImage> images(fileCount);
	cout << endl;

	unsigned char* pixOut = assembled.getPixels();
	unsigned char* pixIn;
	string filename;
#pragma omp parallel for private(filename, pixIn)
	for (int i=0; i<fileCount; i++) {
		filename = imageSet.getPath(i);
		images[i].loadImage(imageSet.getFile(i));
#pragma omp critical (cout)
		cout << filename << endl;

		int iX = i % entriesPerWidth;
		int iY = i / entriesPerWidth;

		pixIn = images[i].getPixels();
		for (int iRow = 0; iRow < height; iRow++)
			memcpy(pixOut + (iX * width + (iY * height + iRow)* totalWidth) * channels, pixIn  + (iRow * width) * channels, width * channels);
	}
	assembled.saveImage(path + ".png");
	ofSetWindowTitle(path);
	assembled.update();
}
//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	assembled.draw(0,0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key != OF_KEY_ESC)
		load();
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