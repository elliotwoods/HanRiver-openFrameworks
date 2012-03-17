#include "ProjectorPixelSet.h"

using namespace ofxGraycode;

namespace HanRiverLib {
	//---------
	void ProjectorPixelSet::add(const ProCamID & proCamID, const ProCamSet & proCamSet, const DataSet & dataSet) {
		map<uint32_t, DataSet::const_iterator> mapping = dataSet.getMapping();

		ofLogNotice("HanRiverLib::ProjectorPixelSet") << "Adding graycode data from camera " << (int) proCamID.camera << " to projector " << (int) proCamID.projector;
		
		PPID id;
		id.projector = proCamID.projector;
		const ProCam & camera ( proCamSet.at(proCamID.camera) );
		ofVec2f cameraXY;
		ofxRay::Ray camRay;
		camRay.s = camera.getPosition();
		for ( map<uint32_t, DataSet::const_iterator>::iterator it = mapping.begin(); it != mapping.end(); it++ ) {
			id.projectorPixel = (*it->second).projector;
			cameraXY = camera.undistort( (*it->second).getCameraXY() );
			camRay.t = camera.getOrientationQuat() *  ofVec3f(-cameraXY.x, cameraXY.y, -1.0f) + camera.getPosition();
			this->operator[](id).insert( pair<CamID, ofxRay::Ray> (proCamID.camera,  camRay ) );
#ifdef PREVIEW_CAM
			foundCamera.addVertex( camRay.t );
			ofFloatColor col(200,100,100);
			col.setHue( ofMap(proCamID.projector, 0, 6, 0, 360) );
			foundCamera.addColor( col );
#endif
		}
	}

	//---------
	void ProjectorPixelSet::clear() {
		map<PPID, RayIntersect>::clear();
		this->points.clear();
		this->foundCamera.clear();
	}
	//---------
	void ProjectorPixelSet::findCameraPoints() {
		this->points.clear();
		ProjectorPixelSet::iterator it;
		ofFloatColor color(200,100,100);
		for (it = this->begin(); it != this->end() ; it++) {
			if (it->second.size() > 1) {
				points.addVertex( it->second.getCrossover() );
				color.setHue( ofMap(it->first.projector, 0, 4, 0, 360) );
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
		/*
		ofMesh rays;
		for (ProjectorPixelSet::const_iterator it = this->begin(); it != this->end(); it++) {
			if (it->second.size() >=2) {
				RayIntersect::const_iterator it2 = it->second.begin();
				rays.addVertex(it2->second.s);
				rays.addVertex(it2->second.t);
				it2++;
				rays.addVertex(it2->second.s);
				rays.addVertex(it2->second.t);
				ofFloatColor col(255, 100, 100);
				col.setHue(ofMap(it->first.projector, 0, 6, 0, 360) );
				rays.addColors( vector<ofFloatColor>(4, col) );
			}
			it->second.draw();
			for (int i=0; i < 10; i++) {
				it++;
				if (it == this->end())
					break;
			}
		}
		rays.setMode(OF_PRIMITIVE_LINES);
		rays.draw();
		*/
	}
}