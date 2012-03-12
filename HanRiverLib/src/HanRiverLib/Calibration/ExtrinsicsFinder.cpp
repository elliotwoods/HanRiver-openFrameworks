#include "ExtrinsicsFinder.h"

namespace HanRiverLib {
	//----------
	void ExtrinsicsFinder::solve(const CameraSetContent & cameraSet) {
		
		this->imagePoints = &cameraSet;
		this->camerasVector = cameraSet.getCamerasAsVector();

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
	}

	//----------
	void ExtrinsicsFinder::findCommonImageSets() {
		int cameraID1, cameraID2;
		for (int i=0; i<camerasVector.size(); i++)
			for (int j=i+1; j<camerasVector.size(); j++) {
				cameraID1 = camerasVector[i]->getCameraID();
				cameraID2 = camerasVector[j]->getCameraID();
				commonImageSets[CameraPair(cameraID1, cameraID2)] = camerasVector[i]->getCommonSuccessfulFinds(*camerasVector[j]);
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
		int firstCameraID = this->camerasVector[0]->getCameraID();
		vector<ofPtr<CameraHead> >::const_iterator it = this->camerasVector.begin();
		it++;
		for (; it != this->camerasVector.end() ; it++) {
			int destination = (**it).getCameraID();
			ofxTSP::RouteFind routeFinder(firstCameraID, destination);
			calibrationRoutes.insert( pair<int, vector<int> >(destination, routeFinder.solve(problem)) );
			ofLogNotice() << "Extrinsics of camera " << destination << " will be solved with route " << calibrationRoutes[destination];
		}
	}

	//----------
	void ExtrinsicsFinder::selectExtrinsicsCalibrations() {
		vector<map<CameraPair, ofMatrix4x4>::iterator > interCamExtrinsicsIterators;
		{
			map<int, ofxTSP::Route >::const_iterator it;
			//go through all routes
			for (it = calibrationRoutes.begin(); it != calibrationRoutes.end(); it++) {
				//step through route
				for (int i=1; i<it->second.size(); i++) {
					CameraPair pair(it->second[i-1], it->second[i]);
					//if this step isn't available
					if (interCamExtrinsics.count(pair) == 0)
						//add it to map
						interCamExtrinsics.insert( std::pair<CameraPair, ofMatrix4x4>(pair, ofMatrix4x4()) );
				}
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
		//given a camera pair identity
		//get image points
		set<int> & commonImagePoints ( this->commonImageSets.at(cameraPair) );
		vector<vector<Point2f> > imagePoints1 = imagePoints->getCameraByID(cameraPair.first).getImagePoints(commonImagePoints);
		vector<vector<Point2f> > imagePoints2 = imagePoints->getCameraByID(cameraPair.second).getImagePoints(commonImagePoints);

		Mat cameraMatrix1, cameraMatrix2;
		Mat distortion1, distortion2;

		imagePoints->getCameraByID(cameraPair.first).getCalibration(cameraMatrix1, distortion1);
		imagePoints->getCameraByID(cameraPair.second).getCalibration(cameraMatrix2, distortion2);

	/*	cv::stereoCalibrate(GlobalBoardFinder::objectPoints, imagePoints1, imagePoints2,
			cameraMatrix1, distortion1,
			cameraMatrix2, distortion2,
			
*/

	}
}
