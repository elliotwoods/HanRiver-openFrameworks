#include "ExtrinsicsFinder.h"

namespace HanRiverLib {
	void ExtrinsicsFinder::solve(const map<int, CameraHead*> & cameraMap, const vector<CameraHead> & cameraVec) {
	
		//create map of vector of pairs
		map<CameraPair, set<int> > imagePointPairIndices;
		for (int i=0; i<cameraVec.size(); i++)
			for (int j=i+1; j<cameraVec.size(); j++)
				imagePointPairIndices[CameraPair(i, j)] = cameraVec[i].getCommonSuccessfulFinds(cameraVec[j]);

		//route find either
		//a) maximum number of pairs
		//b) minimum number of steps with > MINPAIRCOUNT
		//c) journey cost = 1/number of pairs. i.e. many steps is high, many pairs per step is low

		//----------
		//--METHOD C

		//define the problem and journey costs
		ofxTSP::Problem problem;
		{
			map<CameraPair, set<int> >::const_iterator it;
			for (it = imagePointPairIndices.begin(); it != imagePointPairIndices.end(); it++)
				problem.addJourneyCost( it->first.first, it->first.second, 1.0f / (float) it->second.size() );
		}
		problem.nodeCount = cameraVec.size();

		//create a set of calibration routes
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

		//create a map of all stereo calibrations that need to be performed
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

		//perform required calibrations
	

		//integrate calibration routes
	}
}