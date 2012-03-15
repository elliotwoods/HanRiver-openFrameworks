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
	void createReflection();
	void drawMesh();
	void setTexCd(int index, const ofMatrix4x4 & viewProj);
	void drawReflection();
	void loadMesh();
	void drawBack();
	void createBounds();

	ofMesh mesh;
	ofFbo reflected;
	ofMesh bottomPlane;
	ofMesh backPlane;
};