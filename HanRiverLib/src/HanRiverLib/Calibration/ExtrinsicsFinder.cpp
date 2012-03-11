#include "ExtrinsicsFinder.h"

namespace HanRiverLib {
	//----------
	void ExtrinsicsFinder::solve(const map<int, CameraHead*> & cameraMap, const vector<CameraHead> & cameraVec) {
		//route find either
		//a) maximum number of pairs
		//b) minimum number of steps with > MINPAIRCOUNT
		//c) journey cost = 1/number of pairs. i.e. many steps is high, many pairs per step is low

		//----------
		//--METHOD C

		this->findCommonImageSets(cameraVec);
		this->defineJourneys(cameraVec);
		this->findCalibrationRoutes(cameraMap);
		this->selectExtrinsicsCalibrations();
		this->calcExtrinsics(cameraMap);
	}

	//----------
	void ExtrinsicsFinder::findCommonImageSets(const vector<CameraHead> & cameraVec) {
		int cameraID1, cameraID2;
		for (int i=0; i<cameraVec.size(); i++)
			for (int j=i+1; j<cameraVec.size(); j++) {
				cameraID1 = cameraVec[i].getCameraID();
				cameraID2 = cameraVec[j].getCameraID();
				commonImageSets[CameraPair(cameraID1, cameraID2)] = cameraVec[i].getCommonSuccessfulFinds(cameraVec[j]);
			}
	}

	//----------
	void ExtrinsicsFinder::defineJourneys(const vector<CameraHead> & cameraVec) {
		this->problem.clear();

		//define the problem and journey costs
		{
			map<CameraPair, set<int> >::const_iterator it;
			for (it = commonImageSets.begin(); it != commonImageSets.end(); it++)
				problem.addJourneyCost( it->first.first, it->first.second, 1.0f / (float) it->second.size() );
		}

		problem.nodeCount = cameraVec.size();
	}

	//----------
	void ExtrinsicsFinder::findCalibrationRoutes(const map<int, CameraHead*> cameraMap) {
		int firstCameraID = cameraMap.begin()->first;
		{
			map<int, CameraHead*>::const_iterator it = cameraMap.begin();
			it++;
			for (; it != cameraMap.end() ; it++) {
				int destination = it->first;
				ofxTSP::RouteFind routeFinder(firstCameraID, it->first);
				calibrationRoutes.insert( pair<int, vector<int> >(it->first, routeFinder.solve(problem)) );
				ofLogNotice() << "Extrinsics of camera " << it->first << " will be solved with route " << calibrationRoutes.at(it->first);
			}
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
	void ExtrinsicsFinder::calcExtrinsics(const map<int, CameraHead*> cameraMap) {
		map<CameraPair, ofMatrix4x4>::iterator it;
		for (it = interCamExtrinsics.begin(); it != interCamExtrinsics.end(); it++) {
			this->calcExtrinsics(cameraMap, it->first);
		}
	}
	//----------
	void ExtrinsicsFinder::calcExtrinsics(const map<int, CameraHead*> cameraMap, const CameraPair & cameraPair) {
		//given a camera pair identity
		//get image points
		set<int> & commonImagePoints ( this->commonImageSets.at(cameraPair) );
		vector<vector<Point2f> > imagePoints1 = cameraMap.at(cameraPair.first)->getImagePoints(commonImagePoints);
		vector<vector<Point2f> > imagePoints2 = cameraMap.at(cameraPair.second)->getImagePoints(commonImagePoints);

		Mat cameraMatrix1, cameraMatrix2;
		Mat distortion1, distortion2;

		cameraMap.at(cameraPair.first)->getCalibration(cameraMatrix1, distortion1);
		cameraMap.at(cameraPair.second)->getCalibration(cameraMatrix2, distortion2);

	/*	cv::stereoCalibrate(GlobalBoardFinder::objectPoints, imagePoints1, imagePoints2,
			cameraMatrix1, distortion1,
			cameraMatrix2, distortion2,
			
*/

	}
}
