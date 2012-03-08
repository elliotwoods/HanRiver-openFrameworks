#pragma once
#include <map>
#include <set>

#include "ofxUeye.h"
#include "BoardFrame.h"
#include "ofMain.h"

namespace HanRiverLib {
	class CameraHead : public ofBaseDraws {
	public:
		void init(const ofxUeyeDevice & device);
		void capture(int captureID);
		void solveIntrinsics();
	
		bool getHasIntrinsics() const;
		vector<vector<Point2f> > getImagePoints(const set<int> & captureIDs) const;
		int getCameraID() const;
		const set<int> & getSuccessfulFinds() const;
		set<int> getCommonSuccessfulFinds(const set<int> & otherSuccessfulFinds) const;
		set<int> getCommonSuccessfulFinds(const CameraHead & other) const;

		//ofBaseDraws
		void draw(float x,float y);
		void draw(float x,float y,float w, float h);
		float getHeight();
		float getWidth();
	
	protected:
		void calibrateIntrinsics(const vector<vector<Point2f> > & imagePoints);

		map<int, ofPtr<BoardFrame> > boards;
		set<int> successfulFinds;

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