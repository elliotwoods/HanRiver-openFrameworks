#include "CameraSet.h"

namespace HanRiverLib {
	//----------
	CameraSet::~CameraSet() {
		this->closeDevices();
	}

	//----------
	void CameraSet::openDevices(const vector<ofxUeyeDevice> & devices) {
		CameraHead * newCamera;
		uint16_t cameraID;
		for (int i=0; i<devices.size(); i++) {
			newCamera = new CameraHead();
			cameraID = devices[i].cameraID;
			cameraMap.insert( pair<uint16_t, CameraHead*>( cameraID, newCamera) );
			cameraIndices.push_back(cameraID);
		}

		//initialise in parallel
		#pragma omp parallel for
		for (int i=0; i<cameraIndices.size(); i++)
			cameraMap[cameraIndices[i]]->init(devices[i]);
	}

	//----------
	void CameraSet::closeDevices() {
#pragma omp parallel for
		for (int i=0; i<cameraIndices.size(); i++)
			cameraMap[cameraIndices[i]]->close();
	}
	
	//----------
	void CameraSet::capture(int captureID) {
		int cameraID;
		#pragma omp parallel for
		for (int i=0; i<cameraIndices.size(); i++) {
			cameraID = this->cameraIndices[i];

			#pragma omp critical(ofLog)
			ofLogNotice() << "Capture " << captureID << " on camera " << cameraID << " started";
			cameraMap[cameraID]->capture(captureID);
		}
	}

	//----------
	void CameraSet::load() {
		if (!ofFile::doesFileExist("cameraset")) {
			ofLogError("CameraSet") << "Cannot load cameraset as 'cameraset' file does not exist";
			return;
		}

		uint16_t cameraCount;
		uint16_t cameraID;
		uint16_t width, height;
		CameraHead * newCamera;
		map<uint16_t, ofPtr<CameraHead> >::iterator it;
		
		ifstream file(ofToDataPath("cameraset"), ios::binary);
		file.read( (char*) & cameraCount, sizeof(cameraCount) );
		file.read( (char*) & width, sizeof(width) );
		file.read( (char*) & height, sizeof(height) );
		ofLogNotice("CameraSet") << "Loading " << cameraCount << " cameras";

		for (int i=0; i<cameraCount; i++) {
			file.read( (char*) & cameraID, sizeof(cameraID) );
			it = cameraMap.find(cameraID);
			if (it == cameraMap.end()) {
				//device doesn't exist
				newCamera = new CameraHead();

				cameraMap.insert(pair<uint16_t, CameraHead*>(cameraID, newCamera) );
				cameraIndices.push_back(cameraID);

				newCamera->init(cameraID, width, height);
				newCamera->load();
			} else {
				it->second->load();
			}
		}
		file.close();
	}

	//----------
	void CameraSet::save() {
		if (this->size() == 0) {
			ofLogError("CameraSet") << "Save : cannot save as there are 0 cameras";
			return;
		}
		ofstream file(ofToDataPath("cameraset"), ios::binary);
		uint16_t cameraCount = this->cameraIndices.size();
		uint16_t cameraID;

		uint16_t width = this->getFirstCamera().getWidth();
		uint16_t height = this->getFirstCamera().getHeight();

		file.write( (char*) & cameraCount, sizeof(cameraCount) );
		file.write( (char*) & width, sizeof(width) );
		file.write( (char*) & height, sizeof(height) );
		for (int i=0; i<cameraIndices.size(); i++) {
			cameraID = cameraIndices[i];
			file.write( (char*) & cameraID, sizeof(cameraID));
			cameraMap[cameraID]->save();
		}
		file.close();
	}

	//----------
	void CameraSet::solveIntrinsics() {
		int size = this->cameraIndices.size() ;
		ofLogNotice("HanRiverLib::CameraSet") <<"Solving intrinsics for " << size << " cameras";
#pragma omp parallel for
		for (int i=0; i<size; i++) {
#pragma omp critical(ofLog)
			ofLogNotice("HanRiverLib::CameraSet") << "Solving intrinsics for camera " << this->cameraMap[cameraIndices[i]]->getCameraID();
			this->cameraMap[cameraIndices[i]]->solveIntrinsics();
		}
	}

	//----------
	void CameraSet::solveExtrinsics() {
		if (this->cameraIndices.size() < 2) {
			ofLogError() << "cannot solve extrsinics for less than 2 cameras";
			return;
		}

		if (!this->getAllHaveIntrinsics()) {
			ofLogError() << "Cannot solve extrsinsics as 1 or more cameras does not have intrinsics";
			return;
		}

		ExtrinsicsFinder extrsinics;
		extrsinics.solve(*this);
	}

	//----------
	void CameraSet::solveAndSaveCalibration() {
		if ( !this->getAllHaveIntrinsics() )
			this->solveIntrinsics();
		this->solveExtrinsics();
		/**
		do some saving
		**/
	}

	//----------
	bool CameraSet::getAllHaveIntrinsics() const {
		for (CameraSetConstIterator it = this->cameraMap.begin(); it != this->cameraMap.end() ; it++)
			if (!it->second->getHasIntrinsics())
				return false;

		return true;
	}
}