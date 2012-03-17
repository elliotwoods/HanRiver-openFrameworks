#pragma once
#include "ofxGraycode/DataSet.h"

#include "../Common/ProCamID.h"
#include "../Common/ProPixelID.h"
#include "../ProCam/ProCamSet.h"
#include "RayIntersect.h"

#include <stdint.h>
#include <map>

#define PREVIEW_CAM
namespace HanRiverLib {
	//----------
	class ProjectorPixelSet : public std::map<PPID, RayIntersect>, public ofNode {
	public:
		void add(const ProCamID & proCamID, const ProCamSet & proCamSet, const ofxGraycode::DataSet & dataSet);
		void clear();
		void findCameraPoints();

	protected:
		void customDraw();

		ofMesh points;

#ifdef PREVIEW_CAM
		ofMesh foundCamera;
#endif
	};
}