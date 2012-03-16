#pragma once

#include <set>

#include "ofMain.h"
#include "ofxCvGui.h"

#include "Positions.h"
#include "Layer.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);

		ofxCvGui::Builder gui;
		ofxGrabCam * camera;
		Positions positions;
		
		void loadLayers();
		void clearAll();

		void loadFrame();
		void assignPixels();
		string getPathOfMap(int frame) const;

		vector<Layer> layers;
		map<int, ofImage> loaded;
		string contentPath;
		bool playing;
		int frame;
		int frameCount;
		bool animateCamera;
};