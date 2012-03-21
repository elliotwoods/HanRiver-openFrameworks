#include "testApp.h"
#include "ofAppGlutWindow.h"

ProCamSet proCamSet;
ProjectorPixelSet pixelSet;

void loadGraycode() {
	ofDirectory listDataSets;
	listDataSets.allowExt("DataSet");
	listDataSets.listDir( ofToDataPath("", true) );
#pragma omp parallel for
	for (int i=0; i<listDataSets.size(); i++) {
		cout << ofFilePath::getBaseName( listDataSets.getPath(i) ) << ", ";
		ofxGraycode::DataSet dataSet;
		dataSet.load( listDataSets.getPath(i) );
		HanRiverLib::ProCamID id( dataSet.getFilename() );
#pragma omp critical
		pixelSet.add(id, proCamSet, dataSet);
	}

	cout << endl;

	pixelSet.findCameraPoints();
}

int main() {
	//ofAppGlutWindow window;
	//ofSetupOpenGL(&window, 1200, 900, OF_WINDOW);
	//ofRunApp(new testApp());

	ofSeedRandom();
	ofResetElapsedTimeCounter();

	cout << "loading procamset..." << endl;
	proCamSet.load("procamset");
	proCamSet.enforceXZPlane();
	proCamSet.bakeTransform();

	cout << "loading graycode..." << endl;
	loadGraycode();
	
	cout << "saving bigmap..." << endl;
	pixelSet.saveBigMap("bigmap1.exr");
	cout << "exiting" << endl;
}
