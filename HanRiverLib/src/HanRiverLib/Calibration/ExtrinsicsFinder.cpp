#include "ExtrinsicsFinder.h"

namespace HanRiverLib {
	//----------
	void ExtrinsicsFinder::solve(const CameraSetContent & cameraSet) {
		
		this->imagePoints = &cameraSet;
		this->camerasVector = cameraSet.getCamerasAsVector();
		//
		this->indicesInVector.clear();
		for (int i=0; i<this->camerasVector.size(); i++)
			this->indicesInVector.insert(pair<uint16_t, int>(this->camerasVector[i]->getCameraID(), i));

		//route find either
		//a) maximum number of pairs
		//b) minimum number of steps with > MINPAIRCOUNT
		//c) journey cost = 1/number of pairs. i.e. many steps is high, many pairs per step is low

		//----------
		//--METHOD C

		this->findCommonImageSets();
		this->defineJourneys();
		this->findCalibrationRoutes();
		this->selectExtrinsicsCalibrations();
		this->calcExtrinsics();
		this->integrateExtrinsics();
	}

	//----------
	void ExtrinsicsFinder::findCommonImageSets() {
		int cameraID1, cameraID2;
		for (int i=0; i<camerasVector.size(); i++)
			for (int j=i+1; j<camerasVector.size(); j++) {
				commonImageSets[CameraPair(i, j)] = camerasVector[i]->getCommonSuccessfulFinds(*camerasVector[j]);
			}
	}

	//----------
	void ExtrinsicsFinder::defineJourneys() {
		this->problem.clear();

		//define the problem and journey costs
		//divide by zero problem
		//journey map can't be sparse! or start at 1!
		{
			map<CameraPair, set<int> >::const_iterator it;
			for (it = commonImageSets.begin(); it != commonImageSets.end(); it++)
				problem.addJourneyCost( it->first.first, it->first.second, 1.0f / (float) it->second.size() );
		}

		problem.nodeCount = this->camerasVector.size();
	}

	//----------
	void ExtrinsicsFinder::findCalibrationRoutes() {
		//find route from first to all others
		int firstCameraID = imagePoints->getFirstCamera().getCameraID();
		
		for (int i=0; i<camerasVector.size() ; i++) {
			int destination = camerasVector[i]->getCameraID();
			ofxTSP::RouteFind routeFinder(0, i);
			calibrationRoutes.insert( pair<int, ofxTSP::Route >(destination, routeFinder.solve(problem)) );
			if (calibrationRoutes[destination].size() == 0)
				ofLogWarning() << "Extrinsics of camera " << destination << " CANNOT BE SOLVED (no route available)";
			else
				ofLogNotice() << "Extrinsics of camera " << destination << " will be solved with route " << calibrationRoutes[destination];
		}
	}

	//----------
	void ExtrinsicsFinder::selectExtrinsicsCalibrations() {
		interCamExtrinsics.clear();
		map<uint16_t, ofxTSP::Route >::const_iterator it;
		//go through all routes
		for (it = calibrationRoutes.begin(); it != calibrationRoutes.end(); it++) {
			//step through route
			for (int i=1; i<it->second.size(); i++) {
				CameraPair pair(camerasVector[it->second[i-1]]->getCameraID(), camerasVector[it->second[i]]->getCameraID());
				//if this step isn't available
				if (interCamExtrinsics.count(pair) == 0)
					//add it to map
					interCamExtrinsics.insert( std::pair<CameraPair, ofMatrix4x4>( pair, ofMatrix4x4() ) );
			}
		}
	}

	//----------
	void ExtrinsicsFinder::calcExtrinsics() {
		map<CameraPair, ofMatrix4x4>::iterator it;
		for (it = interCamExtrinsics.begin(); it != interCamExtrinsics.end(); it++)
			this->calcExtrinsics(it->first);
	}

	//----------
	void ExtrinsicsFinder::calcExtrinsics(const CameraPair & cameraPair) {
#ifdef HAS_BOARD_FINDER
		//given a camera pair identity
		//get image points
		CameraPair cameraPairInVectorIndices(this->indicesInVector[cameraPair.first], this->indicesInVector[cameraPair.second]);
		set<int> & commonImagePoints ( this->commonImageSets.at(cameraPairInVectorIndices) );

		Mat cameraMatrix1, cameraMatrix2;
		Mat distortion1, distortion2;

		Mat rotation, translation;
		Mat essential, fundamental;

		vector<vector<Point2f> > imagePoints1 = imagePoints->getCameraByID(cameraPair.first).getImagePoints(commonImagePoints);
		vector<vector<Point2f> > imagePoints2 = imagePoints->getCameraByID(cameraPair.second).getImagePoints(commonImagePoints);

		imagePoints->getCameraByID(cameraPair.first).getCalibration(cameraMatrix1, distortion1);
		imagePoints->getCameraByID(cameraPair.second).getCalibration(cameraMatrix2, distortion2);
		vector<vector<Point3f> > boardPoints(imagePoints1.size(), GlobalBoardFinder::objectPoints);

		double error = cv::stereoCalibrate(boardPoints, imagePoints1, imagePoints2,
			cameraMatrix1, distortion1,
			cameraMatrix2, distortion2,
			imagePoints->getImageSize(),
			rotation, translation,
			essential, fundamental);

#pragma omp critical(ofLog)
		ofLogNotice("HanRiverLib::ExtrinsicsFinder") << "Found extrinsics between cameras " << cameraPair.first << " and " << cameraPair.second << ". error = " << error;

		interCamExtrinsics[cameraPair] = ofxCv::makeMatrix(rotation, translation);
#endif
	}

	//----------
	void ExtrinsicsFinder::integrateExtrinsics() {
		ofMatrix4x4 offset;
		//offset = ofMatrix4x4(camerasVector[0]->getFirstBoardTransform()).getInverse();
		offset.makeIdentityMatrix();
		
		for (int i=0; i<camerasVector.size(); i++) {
			ofxTSP::Route route(calibrationRoutes.at(camerasVector[i]->getCameraID()));
			ofxTSP::Route::const_iterator it = route.begin();
			uint16_t cameraID1, cameraID2;
			cameraID1 = camerasVector[*it]->getCameraID();
			cameraID2 = camerasVector.front()->getCameraID();
			it++; // ignore first camera, i.e. start at first step
			ofMatrix4x4 transform = offset;
			for (; it != route.end(); it++) {
				cameraID2 = camerasVector[*it]->getCameraID();
				transform.preMult(interCamExtrinsics[CameraPair(cameraID1, cameraID2)]);
				cameraID1 = cameraID2;
			}
			transform.preMultRotate( ofQuaternion(180, ofVec3f(0,1,0)) );
			this->imagePoints->setCameraExtrinsics(cameraID2, transform);
		}
	}
}
