#include "Positions.h"

//---------
void Positions::load() {
	this->clear();

	ofFile positionsFile;
	positionsFile.open("positions");
	ofVec3f newVec;
	while (!positionsFile.eof()) {
		positionsFile >> newVec;
		this->push_back(newVec);
	}
	this->createBounds();
	this->loadMesh();
	fbo.allocate(map.getWidth(), map.getHeight());
}

//---------
void Positions::customDraw() {
	if (this->map.isAllocated() && this->map.isUsingTexture()) {
		this->map.bind();
		this->mesh.draw();
		this->map.unbind();
	} else
		this->mesh.draw();
}

#define WIDTH 0.02f
#define HEIGHT 0.05f

//---------
ofVec2f ttl(ofRectangle &rect) {
	return ofVec2f(rect.x, rect.y);
}
//---------
ofVec2f ttr(ofRectangle &rect) {
	return ofVec2f(rect.x + rect.width, rect.y);
}
//---------
ofVec2f tbl(ofRectangle &rect) {
	return ofVec2f(rect.x, rect.y + rect.height);
}
//---------
ofVec2f tbr(ofRectangle &rect) {
	return ofVec2f(rect.x + rect.width, rect.y + rect.height);
}
//---------
void Positions::loadMesh() {
	this->mesh.clear();

	ofVec3f tl = ofVec3f(-WIDTH / 2.0f, +HEIGHT / 2.0f, 0.0f);
	ofVec3f tr = ofVec3f(+WIDTH / 2.0f, +HEIGHT / 2.0f, 0.0f);
	ofVec3f bl = ofVec3f(-WIDTH / 2.0f, -HEIGHT / 2.0f, 0.0f);
	ofVec3f br = ofVec3f(+WIDTH / 2.0f, -HEIGHT / 2.0f, 0.0f);

	ofVec3f center;
	ofRectangle subImage;
	for (int i=0; i<this->size(); i++) {
		center = this->at(i);
		subImage = subImageBounds[i];

		mesh.addVertex(center + tl); mesh.addTexCoord(ttl(subImage));
		mesh.addVertex(center + bl); mesh.addTexCoord(tbl(subImage));
		mesh.addVertex(center + tr); mesh.addTexCoord(ttr(subImage));
		
		mesh.addVertex(center + bl); mesh.addTexCoord(tbl(subImage));
		mesh.addVertex(center + br); mesh.addTexCoord(tbr(subImage));
		mesh.addVertex(center + tr); mesh.addTexCoord(ttr(subImage));
	}
}

//---------
void Positions::createBounds() {
	int count = this->size();

	countX = MAP_WIDTH / FACE_RES_X;
	countY =  ceil( (float) count / (float) countX );
	resX = MAP_WIDTH;
	resY = countY * FACE_RES_Y;

	map.allocate(resX, resY, OF_IMAGE_COLOR_ALPHA);
	subImageBounds.clear();
	int iX, iY;
	for (int i = 0; i<count; i++) {
		iX = i % countX;
		iY = i / countX;

		subImageBounds.push_back( ofRectangle(iX * FACE_RES_X, iY * FACE_RES_Y, FACE_RES_X, FACE_RES_Y) );
	}
}