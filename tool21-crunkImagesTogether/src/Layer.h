#pragma once
#include "defines.h"

#include "ofMain.h"
#include "Positions.h"

class Layer : public ofTexture {
public:
	Layer(const Positions & positions, int index);
	void load(string path);
	void setFrame(int frame);
	void update();
	int count() const;
	void createMesh(const Positions & positions);
	void drawBits();

	ofPixels pixels;

	ofDirectory dir;
	vector<int> fileIndices;
	ofMesh mesh;
	int index;
	const Positions & positions;
};

static ofVec2f scaleInLayer; 