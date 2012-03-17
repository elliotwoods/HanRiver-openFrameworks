#include "ProjectorPixelSet.h"

using namespace ofxGraycode;

namespace HanRiverLib {
	//---------
	void ProjectorPixelSet::add(const ProCamID & proCamID, const ProCamSet & proCamSet, const DataSet & dataSet) {

		ofLogNotice("HanRiverLib::ProjectorPixelSet") << "Adding graycode data from camera " << (int) proCamID.camera << " to projector " << (int) proCamID.projector;
		
		PPID id;
		id.projector = proCamID.projector;
		const ProCam & camera ( proCamSet.at(proCamID.camera) );
		ofVec2f cameraXY;
		ofxRay::Ray camRay;

		map<uint32_t, DataSet::const_iterator> mapping = dataSet.getMapping();
		for ( map<uint32_t, DataSet::const_iterator>::iterator it = mapping.begin(); it != mapping.end(); it++ ) {
			id.projectorPixel = (*it->second).projector;
			cameraXY = camera.undistort( (*it->second).getCameraXY() );
			camRay.s = ofVec3f(0,0,0);
			camRay.t = -ofVec3f(-cameraXY.x, -cameraXY.y, +1.0f);
			camRay *= camera.getGlobalTransformMatrix();
			this->operator[](id).insert( pair<CamID, ofxRay::Ray> (proCamID.camera,  camRay ) );
#ifdef PREVIEW_CAM
			ofColor col(200,100,100);
			col.setHue( ofMap( (float) proCamID.projector, 0, 6, 0, 360) );
			foundCamera.addColor( col );
			foundCamera.addVertex( camRay.s + camRay.t );
#endif
		}
	}

	//---------
	void ProjectorPixelSet::calibrateAndAdd(const ProCamID & proCamID, ProCamSet & proCamSet, const DataSet & dataSet) {
		//find common points
		//make 2d camera -> 3d projector pixel correspondences
		//use calibratecamera to find intrinsics + extrinsics of this new camera
		//update ProCamSet
		//perform add
		ofLogNotice("HanRiverLib::ProjectorPixelSet") << "Calibrating camera " << (int) proCamID.camera << " from pixels in projector " << (int) proCamID.projector;

		vector<Point2f> imagePoints;
		vector<Point3f> objectPoints;

		PPID id;
		id.projector = proCamID.projector;
		ProCam * camera = & proCamSet.at(proCamID.camera);

		////
		//find common points
		map<uint32_t, DataSet::const_iterator> mapping = dataSet.getMapping();
		for ( map<uint32_t, DataSet::const_iterator>::iterator it = mapping.begin(); it != mapping.end(); it++ ) {
			id.projectorPixel = it->first;
			if (this->count(id) == 0)
				continue;
			ofVec2f imagePoint = (*it->second).getCameraXY();
			ofVec3f objectPoint = this->at(id).getCrossover();
			imagePoints.push_back( * (Point2f*) & imagePoint );
			objectPoints.push_back( * (Point3f*) & objectPoint );
		}
		//
		////


		////
		//calibrate camera
		cv::Mat cameraMatrix = camera->intrinsics.cameraMatrix;
		cv::Mat distortion = camera->intrinsics.distortion;
		cv::Mat translation;
		cv::Mat rotation;
		cv::solvePnPRansac(objectPoints, imagePoints, cameraMatrix, distortion, rotation, translation, false);

		ofMatrix4x4 view = ofxCv::makeMatrix(rotation, translation);
		cout << "Found view: " << view << endl;
		cout << "\t inverse view: " << view.getInverse() << endl;
		cout << "\t translation: " << translation << ", old: " << camera->getPosition() << endl;
		cout << "\t rotation: " << rotation << ", old: " << camera->getOrientationEuler() << endl;

		////
		//update ProCam
		// set matrix to 
		view.makeInvertOf(view);
		view.preMultRotate(ofQuaternion(180, ofVec3f(0, 1.0f, 0.0f) ) );
		camera->setTransformMatrix(view);
		//
		////


		////
		//add
		this->add(proCamID, proCamSet, dataSet);
		//
		////
	}

	/*
	//---------
	void ProjectorPixelSet::calibrateProjector(const ProCamID & proCamID, ProCamSet & proCamSet, const DataSet & dataSet) {
		//find common points
		//make 2d camera -> 3d projector pixel correspondences
		//use calibratecamera to find intrinsics + extrinsics of this new camera
		//update ProCamSet
		//perform add
		ofLogNotice("HanRiverLib::ProjectorPixelSet") << "Calibrating camera " << (int) proCamID.camera << " from pixels in projector " << (int) proCamID.projector;

		vector<vector<Point2f> > imagePoints(1);
		vector<vector<Point3f> > objectPoints(1);

		PPID id;
		id.projector = proCamID.projector;
		const ProCam * camera = & proCamSet.at(proCamID.camera);

		////
		//find common points
		map<uint32_t, DataSet::const_iterator> mapping = dataSet.getMapping();
		for ( map<uint32_t, DataSet::const_iterator>::iterator it = mapping.begin(); it != mapping.end(); it++ ) {
			id.projectorPixel = it->first;
			if (this->count(id) == 0)
				continue;
			ofVec2f imagePoint = (*it->second).getCameraXY();
			ofVec3f objectPoint = this->at(id).getCrossover();
			imagePoints[0].push_back( * (Point2f*) & imagePoint );
			objectPoints[0].push_back( * (Point3f*) & objectPoint );
		}
		//
		////


		////
		//calibrate camera
		cv::Mat cameraMatrix = camera->intrinsics.cameraMatrix;
		cv::Mat distortion = camera->intrinsics.distortion;
		vector<cv::Mat> translation;
		vector<cv::Mat> rotation;
		float error = cv::calibrateCamera(objectPoints, imagePoints, cv::Size( camera->getWidth(), camera->getHeight() ), cameraMatrix, distortion, rotation, translation, cv::CALIB_USE_INTRINSIC_GUESS);

		ofLogNotice("HanRiverLib::ProjectorPixelSet") << "Calibration complete. Error = " << error;

		ofMatrix4x4 projection;
		projection.makeIdentityMatrix();
		float w = camera->getWidth();
		float h = camera->getHeight();
		float fx = cameraMatrix.at<double>(0, 0);
		float fy = cameraMatrix.at<double>(1, 1);
		float cx = cameraMatrix.at<double>(0, 2);
		float cy = cameraMatrix.at<double>(1, 2);
		float nearDist = 0.01f;
		float farDist = 10.0f;
		projection.makeFrustumMatrix(
			nearDist * (-cx) / fx, nearDist * (w -cx) / fx,
			nearDist * (cy - h) / fy, nearDist * (cy) / fy,
			nearDist, farDist);

		CVIntrinsics intrinsics;
		intrinsics.cameraMatrix = cameraMatrix;
		intrinsics.distortion = distortion;
		intrinsics.undistortedCameraMatrix = cameraMatrix;
		//
		////

		////
		//update procamset
		proCamSet.at(proCamID.camera) = ProCam( ofxCv::makeMatrix(rotation[0], translation[0]), projection, camera->getWidth(), camera->getHeight(), intrinsics);
		//
		////


		////
		//add
		this->add(proCamID, proCamSet, dataSet);
		//
		////
	}
	*/

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
		ofColor color(200,100,100);
		ofVec3f position;
		for (it = this->begin(); it != this->end() ; it++) {
			if (it->second.size() > 1) {
				points.addVertex( it->second.getCrossover() );
				color.setHue( ofMap(it->first.projector, 0, 6, 0, 360) );
				points.addColor(color);
			}
		}
	}

	//---------
	void ProjectorPixelSet::saveRaw(string filename) {
		if (filename == "")
			filename = ofSystemSaveDialog("xyz.raw", "Save raw points").getPath();
		if (filename == "") {
			ofLogWarning("ProjectorPixelSet") << "No file selected for raw save";
			return;
		}

		ofstream file(ofToDataPath(filename), ios::binary);
		vector<ofVec3f> points = this->points.getVertices();
		uint32_t size = points.size();
		file.write( (char*) & size, sizeof(size) );
		file.write( (char*) & points[0], sizeof(ofVec3f) * size);
		file.close();
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