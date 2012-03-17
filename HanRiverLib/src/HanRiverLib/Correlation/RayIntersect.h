#pragma once
#include "ofMain.h"
#include "ofxRay.h"
#include "ofxGraycode.h"

#include "../Common/ID.h"

#include <vector>
#include <limits>

using namespace std;

namespace HanRiverLib {
	class RayIntersect : public map<CamID, ofxRay::Ray> {
	public:
		void draw() const;
		ofVec3f getCrossover();
	};
}