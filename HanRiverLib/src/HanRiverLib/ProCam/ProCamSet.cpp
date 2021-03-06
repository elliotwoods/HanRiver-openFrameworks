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

			file.write( (char*) & index, sizeof(index) );
			file.write( (char*) & width, sizeof(width) );
			file.write( (char*) & height, sizeof(height) );
			file.write( (char*) & view, sizeof(view) );
			file.write( (char*) & projection, sizeof(projection) );
			it->second.intrinsics.saveTo(file);
		}

		file.close();
	}

	//---------
	void ProCamSet::load(string filename) {
		if (filename=="")
			filename = ofSystemLoadDialog("Load ProCamSet").getPath();
		
		if ( ! ofFile::doesFileExist(filename) ) {
			ofLogError("HanRiverLib::ProCamSet") << "ProCamSet cannot find file " << filename;
			return;
		}

		uint16_t count = this->size();
		uint16_t index;
		ofMatrix4x4 view, projection;
		uint16_t width, height;
		CVIntrinsics intrinsics;
		ifstream file(ofToDataPath(filename), ios::binary);
		file.read( (char*) & count, sizeof(count) );
		this->clear();
		for (uint16_t i = 0; i < count; i++) {
			file.read( (char*) & index, sizeof(index) );
			file.read( (char*) & width, sizeof(width) );
			file.read( (char*) & height, sizeof(height) );
			file.read( (char*) & view, sizeof(view) );
			file.read( (char*) & projection, sizeof(projection) );
			intrinsics.loadFrom(file);
			
			this->insert(pair<uint16_t, ProCam>( index, ProCam(view, projection, width, height, intrinsics) ) );
		}

		file.close();
	}

	//---------
	void ProCamSet::enforceXZPlane() {
		//move so camera 1 is at origin
		this->move( -this->begin()->second.getPosition() );

		//move last camera so it's in +x direction
		ProCamSet::iterator it = this->end();
		it--;
		ofVec3f lastCamera = it->second.getPosition() * this->getGlobalTransformMatrix();
		ofQuaternion toXAxis;
		toXAxis.makeRotate( lastCamera.normalized(), ofVec3f(1,0,0) );
		this->rotate(toXAxis);

		//find biggest outlier in Y and rotate to move it into xz plane
		ofVec3f biggestY(0,0,0);
		ofVec3f current;
		for (it = this->begin(); it != this->end(); it++) {
			current = it->second.getPosition() * this->getGlobalTransformMatrix();
			if (abs(current.y) > abs(biggestY.y))
				biggestY = current; //this should generally be camera 3
		}
		if (biggestY.y != 0.0f) {
			//rotate on X axis
			biggestY.x = 0.0f;
			biggestY.normalize();
			toXAxis.makeRotate( biggestY, ofVec3f(0,0,1) );
			this->rotate(toXAxis);
		}
	}

	//---------
	void ProCamSet::bakeTransform() {
		ofMatrix4x4 global = this->getGlobalTransformMatrix();
		ofMatrix4x4 local;
		for (ProCamSet::iterator it = this->begin(); it != this->end() ; it++ ) {
			local = it->second.getGlobalTransformMatrix();
			local.postMult(global);
			it->second.setTransformMatrix(local);
		}
		this->setTransformMatrix( ofMatrix4x4() );
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
			ofPopMatrix();
			ofPopStyle();
		}
	}
}