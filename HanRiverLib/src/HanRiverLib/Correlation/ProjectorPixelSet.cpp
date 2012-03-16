#include "ProjectorPixelSet.h"

using namespace ofxGraycode;

namespace HanRiverLib {
	//---------
	void ProjectorPixelSet::add(const ProCamID & proCamID, const ProCamSet & proCamSet, const DataSet & dataSet) {
#ifdef PREVIEW_CAM
		foundCamera.clear();
#endif
		map<uint32_t, DataSet::const_iterator> mapping = dataSet.getMapping();

		PPID id;
		id.projector = proCamID.projector;
		ofVec2f cameraXY;

		for ( map<uint32_t, DataSet::const_iterator>::iterator it = mapping.begin(); it != mapping.end(); it++ ) {
			id.projectorPixel = (*it->second).projector;
			cameraXY = proCamSet.at(proCamID.camera).undistort( (*it->second).getCameraXY() );
			this->operator[](id).insert( pair<CamID, ofxRay::Ray> (proCamID.camera,  proCamSet.at(proCamID.camera).castCoordinate(-cameraXY) ) );
#ifdef PREVIEW_CAM
			foundCamera.addVertex( cameraXY );
#endif
		}
	}

	//---------
	void ProjectorPixelSet::findCameraPoints() {
		this->points.clear();
		ProjectorPixelSet::iterator it;
		ofFloatColor color(200,100,100);
		for (it = this->begin(); it != this->end() ; it++) {
			if (it->second.size() > 1) {
				points.addVertex( it->second.getCrossover() );
				color.setHue( ofMap(it->first.projector, 0, 10, 0, 360) );
				points.addColor(color);
			}
		}
	}

	//---------
	void ProjectorPixelSet::customDraw() {
#ifdef PREVIEW_CAM
		foundCamera.drawVertices();
#endif
		glPointSize(2.0f);
		this->points.drawVertices();
	}
}