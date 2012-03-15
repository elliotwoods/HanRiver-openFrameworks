#pragma once
#include "defines.h"
#include "ofMain.h"

class Positions : public ofNode, public vector<ofVec3f> {
public:
	void load();
	vector<ofRectangle> subImageBounds;
	int countX, countY;
	int resX, resY;
	ofImage map;
	ofFbo fbo;

protected:
	void customDraw();
	void loadMesh();
	void createBounds();

	ofMesh mesh;
};