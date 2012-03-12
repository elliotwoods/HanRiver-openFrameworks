#pragma once

#include "ofMain.h"
#include <stdint.h>
#include <valarray>
#include <map>

#include "ofxGraycode.h"
#include "ofxProCamSolver.h"

using namespace ofxProCamSolver;
using namespace ofxGraycode;

///Projector pixel
class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);

		CorrespondenceSetd correspondences;
		int camera;
		int projector;

		void addProjector();
		void addCamera();
};
