#pragma once

#include "ofMain.h"
#include "ofxGrabCam.h"
#include "ofxRay.h"
#include "projCamPair.h"


class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void loadImageCubes();
	void loadPixelPoints(string fileName);

	ofFloatImage* selectImageSection(int index, int nsplit);

	projCamPair* setupProjCamPair(int index);
  
	void dumpPoints(string filename, const ofFloatImage& image); // ccs added this to output the data we read as ascii
  
	ofxGrabCam camera;
  
	ofFloatImage imgCubes;
	ofFloatImage imgPixels; // elliots raw data is 4 images
	float maxValue;
	ofVec3f mean;
  
	ofMesh meshCubes;
    
	projCamPair *pair1; // will need to free this somewhere...
	projCamPair *pair2; 
	projCamPair *pair3; 
	projCamPair *pair4;
};

void loadMatrix(ofMatrix4x4 *m, string filepath);