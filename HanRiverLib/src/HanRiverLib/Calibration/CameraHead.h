#pragma once
#include <map>
#include <set>

#include "ofxUeye.h"
#include "ofxUeyePreset_5480ChessBoard.h"

#include "BoardFrame.h"
#include "ofMain.h"
#include "ofxCvGui/Assets.h"

namespace HanRiverLib {
	class CameraHead : public ofBaseDraws {
	public:
		CameraHead();
		~CameraHead();

		void init(uint16_t cameraID); ///<initialise without device
		void init(const ofxUeyeDevice & device);  ///<initialise with device
		void close();

		void capture(int captureID);
		void solveIntrinsics();
	
		bool getHasIntrinsics() const;
		vector<vector<Point2f> > getImagePoints(const set<int> & captureIDs) const;
		int getCameraID() const;
		const set<int> & getSuccessfulFinds() const;
		set<int> getCommonSuccessfulFinds(const set<int> & otherSuccessfulFinds) const;
		set<int> getCommonSuccessfulFinds(const CameraHead & other) const;
		void getCalibration(Mat & cameraMatrix, Mat & distortion) const;

		//ofBaseDraws
		void draw(float x,float y);
		void draw(float x,float y,float w, float h);
		float getHeight();
		float getWidth();
	
		//image points
		void load();
		void save();
	protected:
		void waitForComplete() const;
		void updateSuccessfulFinds();

		void calibrateIntrinsics(const vector<vector<Point2f> > & imagePoints);
		string getFilenameBase() const;
		string getBoardFilename(int board) const;

		//image points
		map<int, ofPtr<BoardFrame> > boards;
		set<int> successfulFinds;

		//device
		bool hasDevice;
		ofxUeye camera;
		bool newFrame;
		int cameraID;
		ofTexture preview;

		//data
		ofxCv::Intrinsics intrinsics;
		Mat distortion;
		float reprojectionError;
		bool hasIntrinsics;
	};
}