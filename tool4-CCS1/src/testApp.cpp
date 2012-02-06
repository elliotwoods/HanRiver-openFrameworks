#include "testApp.h"

/**
 * ccs, copied in fns from elliots project: test0-16bitImageLoad
 * \todo - readme
 *       - proper config options
 *       - variable number of proj + cam
 *       - make the correspondances actually work
 *       - connect to openPCL
 */

//--------------------------------------------------------------
void testApp::setup(){
	pair1 = pair2 = pair3 = pair4 = 0x0;

	maxValue = 0;
	ofBackground(50,50,50);
	ofSetLogLevel(OF_LOG_NOTICE);
	meshCubes.setMode(OF_PRIMITIVE_POINTS);
	pair1 = setupProjCamPair(1);
	//pair2 = setupProjCamPair(2);  
	//pair3 = setupProjCamPair(3);
	//pair4 = setupProjCamPair(4);
	loadImageCubes();
	ofLogNotice() << "iProjector in camera";
	loadPixelPoints(ofSystemLoadDialog("iProjector in camera").getPath()); // load the pixels
	pair1->setPixels(selectImageSection(0, 4));
	pair1->xyzCubes = imgCubes;
	pair1->procCorrespond();

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	camera.begin();
	ofPushStyle();
	ofSetColor(250,150,150);
	ofDrawGrid();
	ofPopStyle();
	meshCubes.drawVertices();
	if(pair1 != 0x0)
		pair1->draw();
	if(pair2 != 0x0)
		pair2->draw();
	if(pair3 != 0x0)
		pair3->draw();
	if(pair4 != 0x0)
		pair4->draw();  
	camera.end();
  
	int y = 0;
	ofDrawBitmapString("Max length: " + ofToString(maxValue), 10, y+=15);
	ofDrawBitmapString("Mean position: " + ofToString(mean), 10, y+=15);
	ofDrawBitmapString("Vertex count: " + ofToString(meshCubes.getNumVertices()), 10, y+=15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 'u')
		loadImageCubes();
	//  if (key == 'p')
	//		loadPixelPoints();
	if (key == 'c')
		camera.toggleCursorDraw();
	if (key =='f')
		ofToggleFullscreen();
	if (key == 'd')
		dumpPoints("data-dump.dat", imgCubes);
}


projCamPair* testApp::setupProjCamPair(int index){
	int projW = 1024;
	int projH = 768;
	int camW = 2560; // high res!
	int camH = 1920; 
	ofMatrix4x4 pmat; 
	ofMatrix4x4 cmat;
	// hardcoded for testing only
	stringstream filename;
	ofLogNotice() << "Load camera matrix";
	string cMat1 = ofFilePath::removeExt(ofSystemLoadDialog("Load camera matrix").getPath());
	ofLogNotice() << "Load projector matrix";
	string pMat1 = ofFilePath::removeExt(ofSystemLoadDialog("Load projector matrix").getPath());
  
	//strip out the last number
	cMat1.pop_back();
	pMat1.pop_back();
  
	filename << pMat1 << index << ".mat";
	loadMatrix(&pmat, filename.str());
	filename.clear(); // reset err flags
	filename.str(""); // reset sstream contents 
	filename << cMat1 << index << ".mat"; 
	loadMatrix(&cmat, filename.str());
  
	/**
	* this is a fix, the projector matrix is currently rotated by 90*
	* rotated compared to the cam, this willÊundo this
	*/
	ofMatrix4x4 temp = cmat;
	//temp.rotate(-90,0,0,1);
  
	// ccs
	// index sets the chanel index of the pixelMap that the projector corresponds to
	return(new projCamPair(pmat, projW, projH, temp,  camW, camH, index));
}

void loadMatrix(ofMatrix4x4 *mat, string filepath){    
	ofLogError() << "reading matrix from: " << filepath;
	FILE *fptr = fopen(filepath.c_str(), "r");
	if(fptr == 0x0){
		ofLogError() << "cannot open file: " << filepath; 
	} else {
		float t1, t2, t3, t4;
		for(int i = 0; i < 4; i++){
			fscanf(fptr, "%f,%f,%f,%f,", &t1, &t2, &t3, &t4);
			ofLogError() << i << "read: " << t1 << " " << t2 << " " << t3 << " " << t4;
			(*mat)(i,0) = t1;
			(*mat)(i,1) = t2;  
			(*mat)(i,2) = t3;
			(*mat)(i,3) = t4;
		}
	}

	fclose(fptr);
}

/**
 * initial file is 4 images glued together, that's why its so big
 * we extract one of the subimages
 * @arg index: which image to extract (starting from 0)
 * @arg nsplit: how many images are glued together (in the vertical direction)
 */
ofFloatImage* testApp::selectImageSection(int index, int nsplit){
  // how to we check that the image is full;
  if(imgPixels.getWidth() < 1){
    ofLogError() << "spilt failed on empty image";
    throw(0);
  } else {
    int width = imgPixels.getWidth();
    int height = imgPixels.getHeight()  / nsplit;
    int channels = imgPixels.getPixelsRef().getNumChannels();
      
    int offsetX = 0; 
    int offsetY = index * height;
    
    ofLogError() << "splitting out image: " << index;
    ofLogError() << "width: " << width << " height: " << height << " channels: " << channels;    
    
//    int count = 0;
//    int index = 0;
//    float pval1, pval2, pval3;
//    float *pixVal = (float*)imgPixels.getPixels();

//    while(count < 5 ){
//      pval1 = *pixVal++;
//      pval2 = *pixVal++;
//      pval3 = *pixVal++;
//      if(pval1 != 0 || pval2 != 0 || pval3 !=0){
//        ofLogError() << "index: " << index << "pixel: " << pval1 << " "  << pval2 << " " << pval3;
//        count++;
//      }
//      index++;
//    }
//    
    
    ofFloatImage *img = new ofFloatImage();
    img->cropFrom(imgPixels, offsetX, offsetY, width, height);
    return(img);
  }
}

void testApp::loadPixelPoints(string fileName){
//  ofFileDialogResult res = ofSystemLoadDialog("Select image");
//  string fileName = res.filePath;
  if (!(imgPixels.loadImage(fileName)))
		ofLogError() << "Failed to load image";
  else {
    ofLog() << "Loaded pixel points: " << fileName;
  }
  
}

void testApp::loadImageCubes(){
  float fudge = 5.0; // because for some reason the xyz file is offset by 5...?
  ofLogNotice() << "Select XYZ in camera space";
  ofFileDialogResult res = ofSystemLoadDialog("Select XYZ in camera space");
  string fileName = res.filePath;

  if (!(imgCubes.loadImage(fileName)))
		ofLogError() << "Failed to load image";
	else {
		//fill mesh and find max value
		maxValue = 0;
		mean = (ofVec3f)0;
    
		const int count = imgCubes.getWidth() * imgCubes.getHeight();
		
		ofVec3f* vertex = (ofVec3f*) imgCubes.getPixels();
    ofVec3f tempVert;
		meshCubes.clearVertices();
		float lengthSq;
		
		for (int i=0; i<count; i++, vertex++) {
			lengthSq = vertex->lengthSquared();
      
      tempVert.x = vertex->x - fudge;
      tempVert.y = vertex->y - fudge;
      tempVert.z = vertex->z - fudge;
      
			if (lengthSq != 0) {
				//cout << "add vertex " << *vertex << endl;
				meshCubes.addVertex(tempVert);
				if (lengthSq > maxValue)
					maxValue = lengthSq;
				mean += *vertex;
			}
		}
    
		maxValue = sqrt(maxValue);
		mean /= float(count);
	}
}

void testApp::dumpPoints(string filename, const ofFloatImage& img){
  int count = meshCubes.getNumVertices();
  ofVec3f vertex;
  FILE *fptr = fopen(filename.c_str(), "w");
  cerr << "# dumping points to file" << endl; 
  // write, n, height, width, xyz xyz xyz
  
  fwrite(&count,  sizeof(int), 1, fptr);
  fwrite(&(img.height), sizeof(int), 1, fptr); 
  fwrite(&(img.width), sizeof(int), 1, fptr);
  for(int i =0 ; i < count; i++){
    vertex = meshCubes.getVertex(i);
    //fprintf(fptr, "%f %f %f\n", vertex.x, vertex.y, vertex.z);
    fwrite(&(vertex.x), sizeof(float), 1, fptr);
    fwrite(&(vertex.y), sizeof(float), 1, fptr);  
    fwrite(&(vertex.z), sizeof(float), 1, fptr);
  }
  fclose(fptr);
  cerr << "# done " << endl;
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
