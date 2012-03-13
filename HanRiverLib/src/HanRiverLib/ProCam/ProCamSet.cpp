#include "ProCamSet.h"

namespace HanRiverLib {
	//---------
	ProCamSet::ProCamSet(const CameraSetContent & cameraSet) {
		this->add(cameraSet);
	}

	//---------
	void ProCamSet::add(const CameraSetContent & cameraSet) {
		vector<ofPtr<CameraHead > > calibratedCameras = cameraSet.getCamerasAsVector();
		vector<ofPtr<CameraHead > >::iterator it;
		for (it = calibratedCameras.begin(); it != calibratedCameras.end(); it++)
			this->insert( pair<uint16_t, ProCam>( (*it)->getCameraID(), ProCam(**it) ) );
	}

	//---------
	void ProCamSet::save(string filename) {
		if (filename=="")
			filename = ofSystemSaveDialog("procamset", "Save ProCamSet").getPath();

		uint16_t index;
		uint16_t count = this->size();
		ofMatrix4x4 view, projection;
		const float * distortion;
		uint16_t width, height;

		ofstream file(ofToDataPath(filename), ios::binary);
		file.write( (char*) & count, sizeof(count) );
		ProCamSet::const_iterator it;
		for (it = this->begin(); it != this->end(); it++) {
			index = it->first;
			width = it->second.getWidth();
			height = it->second.getHeight();
			view = it->second.getGlobalTransformMatrix();
			projection = it->second.getProjectionMatrix();
			distortion = it->second.getDistortionPtr();

			file.write( (char*) & index, sizeof(index) );
			file.write( (char*) & width, sizeof(width) );
			file.write( (char*) & height, sizeof(height) );
			file.write( (char*) & view, sizeof(view) );
			file.write( (char*) & projection, sizeof(projection) );
			file.write( (char*) distortion, sizeof(float) * 5 );
		}

		file.close();
	}

	//---------
	void ProCamSet::load(string filename) {
		if (filename=="")
			filename = ofSystemLoadDialog("Load ProCamSet").getPath();

		uint16_t count = this->size();
		uint16_t index;
		ofMatrix4x4 view, projection;
		float distortion[5];
		uint16_t width, height;

		ifstream file(ofToDataPath(filename), ios::binary);
		file.read( (char*) & count, sizeof(count) );
		this->clear();
		for (uint16_t i = 0; i < count; i++) {
			file.read( (char*) & index, sizeof(index) );
			file.read( (char*) & width, sizeof(width) );
			file.read( (char*) & height, sizeof(height) );
			file.read( (char*) & view, sizeof(view) );
			file.read( (char*) & projection, sizeof(projection) );
			file.read( (char*) distortion, sizeof(float) * 5 );
			
			this->insert(pair<uint16_t, ProCam>( index, ProCam(view, projection, width, height, distortion) ) );
		}

		file.close();
	}

	//---------
	void ProCamSet::customDraw() {
		ProCamSet::const_iterator it;
		bool isProjector;
		uint8_t index;
		for (it = this->begin(); it != this->end(); it++) {
			it->second.draw();

			isProjector = it->first >= 1 << 8;
			index = isProjector ? it->first >> 8 : it->first;
			ofPushStyle();
			ofPushMatrix();
			ofTranslate( it->second.getGlobalPosition() );
			ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
			ofDrawBitmapString(isProjector ? "Projector " : "Camera " + ofToString( (int) index), 0, 0, 0);
			ofPopStyle();
		}
	}
}