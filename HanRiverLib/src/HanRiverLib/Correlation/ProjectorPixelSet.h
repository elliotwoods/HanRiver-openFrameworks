#pragma once
#include "ofxGraycode/DataSet.h"

#include "../Common/ProCamPairID.h"
#include "../Common/ProCamPixelID.h"
#include "../ProCam/ProCamSet.h"
#include "RayIntersect.h"

#include <stdint.h>
#include <map>

#define PREVIEW_CAM
namespace HanRiverLib {
	//----------
	class ProjectorPixelSet : public std::map<PPID, RayIntersect>, public ofNode {
	public:
		//add graycode
		void add(const ProCamPairID & proCamPairID, const ProCamSet & proCamSet, const ofxGraycode::DataSet & dataSet);

		//calibrate camera based on existing correspondences
		void calibrateAndAdd(const ProCamPairID & proCamPairID, ProCamSet & proCamSet, const ofxGraycode::DataSet & dataSet);
		
		void clear();
		void findCameraPoints();
		void saveRaw(string filename = "");
		void saveBigMap(string filename = "");
		void saveCompressedMap(string filename = "");
		void saveCorrespondenceTable(string filename = "");
	protected:
		void customDraw();

		ofMesh points;

#ifdef PREVIEW_CAM
		ofMesh foundCamera;
#endif
	};
}