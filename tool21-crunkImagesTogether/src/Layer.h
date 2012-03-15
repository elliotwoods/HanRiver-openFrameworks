#pragma once
#include "defines.h"

#include "ofMain.h"
#include "Positions.h"

class Layer : public ofImage {
public:
	Layer(const Positions & positions, int index);
	void load(string path);
	void setFrame(int frame);
	int count() const;
	void createMesh(const Positions & positions);
	void drawBits();

	ofDirectory dir;
	vector<int> fileIndices;
	ofMesh mesh;
	int index;
	const Positions & positions;
};

static ofVec2f scaleInLayer; 