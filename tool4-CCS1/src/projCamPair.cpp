//
//  projCamPair.cpp
//  testImgLoad
//
//  Created by Christopher Coleman-Smith on 2/3/12.
//  Copyright (c) 2012 Duke Uni. All rights reserved.
//

#include "projCamPair.h"


projCamPair::projCamPair(){
  // init with default for projector class, this will set
  // random poses and locations i guess...
  CaptureProjector = ofProjector();
  CaptureCamera  = ofProjector();
  ncorrespond = 0;
  pointMesh.setMode(OF_PRIMITIVE_POINTS); 
	projectorIndex = 1; // default to the first channel
}

projCamPair::projCamPair(const ofMatrix4x4& pProjView, int projW, int projH, 
                         const ofMatrix4x4 & cProjView, int cameraW, int cameraH,
												 int projectorIndexIn){    

	projectorIndex = projectorIndexIn;
  
  CaptureProjector = ofProjector(pProjView, projW, projH);
  CaptureCamera = ofProjector(cProjView, cameraW, cameraH);
    
  // we should pick colors for both, otherwise may not draw right
  CaptureProjector.color = ofColor(0,0,255);
  CaptureCamera.color= ofColor(255,0,0);  
    
  ofLogError() << "projCamPair: "<< (int)this << "init with: cam(" << cameraW << " x " << cameraH << ") proj (" << projW << " x " << projH << ")";
  ncorrespond = 0;
  pointMesh.setMode(OF_PRIMITIVE_POINTS); 
}

void projCamPair::setPixels(ofFloatImage *pix){
  this->imgPixels = *pix;
}

void projCamPair::setCubes(ofFloatImage *pix){
  this->xyzCubes = *pix;
}


void projCamPair::draw(){
	CaptureProjector.draw();
	CaptureCamera.draw();
  
	if(pointMesh.getNumVertices()){
		pointMesh.drawVertices();
	}

	projRayTest.draw();
	camRayTest.draw();
	interRay.draw();
}

struct corresPixel{ // a nonempty pixel in the correspondance image
	int index; // the index in the camera
	float v1; // values 
	float v2;
	float v3;
};

/**
 * start by finding the non empty pixels in the correspondance image
 * these are saved into the vector nonEmptyPixels
 */
void projCamPair::procCorrespond(){
	ofFloatPixels *pix = (ofFloatPixels*)imgPixels.getPixels();
	float* pixVal = (float*) imgPixels.getPixels();
	ofLogError() << "npix per channell: " << imgPixels.getPixelsRef().getNumChannels();
	std::vector<corresPixel> nonEmptyPixels;
	nonEmptyPixels.reserve(10000); // probably need at least this many...
	std::vector<corresPixel> confusingPixels;
	confusingPixels.reserve(1000);

	corresPixel *tempPix;
	int count = 0;
	int npix = imgPixels.getWidth() * imgPixels.getHeight(); // size of the image
	ofLogError() << "npix: " << npix;

	float pval1, pval2, pval3;
	while(count < npix ){
		pval1 = *pixVal++;
		pval2 = *pixVal++;
		pval3 = *pixVal++;
		if(pval1 != 0 || pval2 != 0 || pval3 !=0){
			tempPix = new corresPixel;
			tempPix->index = count;
			tempPix->v1 = pval1;
			tempPix->v2 = pval2;
			tempPix->v3 = pval3;
			nonEmptyPixels.push_back(*tempPix);      
			//ofLog() << "index: " << count << "pixel: " << pval1 << " "  << pval2 << " " << pval3;
			delete tempPix;
		}
		count++;
	}
	ofLogError() << "N_useful pixels: " << nonEmptyPixels.size();
  
	for(int i = 0; i < 5; i++)
		ofLogError() << nonEmptyPixels[i].index << "  " << nonEmptyPixels[i].v1 << " " << nonEmptyPixels[i].v2 << " " << nonEmptyPixels[i].v3;
	
  
	/**
	* process the apparently valid pixels, for each pixel we will try and extract an index
	* this is presumably the channel value which is non-negative, in the event of multiple
	* non-negative channel values we'll push the pixel to a 'confusingList'
	*/

  /**
   * actually, apparently for each projector, we keep the image map in a different
   * channel, so if we're p1 it'd be channel1, p2 it'll be channel 2 etc
   * this is weird, what if they're more projectors than channels? such is the way of
   * the world 
   */
	float v1, v2, v3;
	int pointIndex = -1;
	int nProcessed = 0; // how many pixels we actually processed
	int nTestProcess = 1; // how many rays should we process before we break?
	// start with just the first 5;
	for(int i = 0; i < nonEmptyPixels.size()  ; i++){
		v1 = nonEmptyPixels[i].v1;
 		v2 = nonEmptyPixels[i].v2;
		v3 = nonEmptyPixels[i].v3;
    pointIndex = -1;
		// if(v1 > 0 && v2 <= 0 && v3 <=0){
		// 	pointIndex = (int)v1;
		// } else if( v2 >= 0 && v1 <=0 && v3 <=0){
		// 	pointIndex = (int)v2;
		// } else if( v3 >= 0 && v1 <=0 && v2 <=0){
		// 	pointIndex = (int)v3;
		// } else {
		// 	pointIndex = -1;
		// }

    switch(projectorIndex){
      case 1:
        if(v1 > 0){
          pointIndex = (int)v1;
        }
        break;
      case 2:
        if(v2 > 0){
          pointIndex = (int)v2;          
        }
        break;
      case 3:
        if(v3 > 0){
          pointIndex = (int)v3;
        }
        break;
      default:
        pointIndex = -1;
        break;
    }

    
		if(pointIndex != -1){
			processPixel(nonEmptyPixels[i].index, pointIndex);
			nProcessed++;
			if(nProcessed > nTestProcess){
				break;
			}
		} else {
			tempPix = new corresPixel;
			tempPix->index = nonEmptyPixels[i].index;
			tempPix->v1 = v1;
			tempPix->v2 = v2;
			tempPix->v3 = v3;
			confusingPixels.push_back(*tempPix);
			delete tempPix;
		}            
	}
	ofLogError() << "N_confusing pixels: " << confusingPixels.size();
	ofLogError() << "N_worked pixels: " << pointMesh.getNumVertices();
	//exit(1);
}

/**
  * we have measured a signal indicating that projectorPixelIndex
  * can be seen by cameraPixelIndex
  * cast rays from the camera and the projector at these locations and
  * compute their intersection
  * if this is a good intersection add it as a 3d point in 
  * into pointMesh
  */
void projCamPair::processPixel(int cameraPixelIndex, int projectorPixelIndex){
	ofVec3f interSecnLocn;
  
	ofRay projectorRay = CaptureProjector.castPixelIndex(projectorPixelIndex);
	ofRay cameraRay = CaptureCamera.castPixelIndex(cameraPixelIndex);

  
//  ofRay projectorRay = CaptureProjector.castPixelIndex(0);
//  ofRay cameraRay = CaptureCamera.castPixelIndex(0);
  
	ofRay intersector = projectorRay.intersect(cameraRay);

	this->projRayTest = projectorRay;
	this->camRayTest = cameraRay;
	this->interRay = intersector;
  
	ofVec3f* cubeLocns = (ofVec3f*)this->xyzCubes.getPixels(); // locn in the cubes map
	ofVec3f posRay = cubeLocns[cameraPixelIndex] - ofVec3f(5.0,5.0,5.0);
  
	this->pointMesh.addVertex(posRay);
  
	ofLogError() << "posnRay: " << posRay;
	ofLogError() << "interSectingRay: " << interRay.s << interRay.t << "length: " << interRay.getLength();
	//  
	if(projectorRay.intersectionPoint(cameraRay, &interSecnLocn)){
		ofLogError() << "intersection: (" << cameraPixelIndex << "," << projectorPixelIndex  << "): " << interSecnLocn.x << " " << interSecnLocn.y << " " << interSecnLocn.z;
		this->pointMesh.addVertex(interSecnLocn);
	} 
}

//void projCamPair::loadPixels(string filename){    
//  if (!(imgPixels.loadPFMImage(filename)))
//		ofLogError() << "projCamPair: Failed to load image";
//  else {
//    ofLog() << "projCamPair: Loaded pixel points: " << filename;
//  }  
//}

void projCamPair::dumpPoints(string filename){
	int count = pointMesh.getNumVertices();
	if(count == 0){
		ofLogError() << "projCamPair: failed to dump points, empty mesh";
	} else {
		ofVec3f vertex;
		FILE *fptr = fopen(filename.c_str(), "w");
		ofLog() << "projCamPair: dumping points to file: " << filename;
		cerr << "# dumping points to file" << endl; 
		// write, n, height, width, xyz xyz xyz
    
		fwrite(&count,  sizeof(int), 1, fptr);
		fwrite(&(imgPixels.height), sizeof(int), 1, fptr); 
		fwrite(&(imgPixels.width), sizeof(int), 1, fptr);
		for(int i =0 ; i < count; i++){
			vertex = pointMesh.getVertex(i);
			//fprintf(fptr, "%f %f %f\n", vertex.x, vertex.y, vertex.z);
			fwrite(&(vertex.x), sizeof(float), 1, fptr);
			fwrite(&(vertex.y), sizeof(float), 1, fptr);  
			fwrite(&(vertex.z), sizeof(float), 1, fptr);
		}
		fclose(fptr);
		ofLog() << "projCamPair: point dump finished";
		cerr << "# done " << endl;
	}
}
