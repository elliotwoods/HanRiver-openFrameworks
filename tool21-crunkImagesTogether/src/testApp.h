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
		Positions positions;
		
		void loadLayers();

		void loadFrame();
		void assignPixels();

		vector<Layer> layers;
		map<int, ofImage> loaded;
		string contentPath;
		bool playing;
		int frame;
		int frameCount;
};