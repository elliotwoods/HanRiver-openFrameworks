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

	this->reflected.allocate(1204, 768);
	bottomPlane.addVertex( ofVec3f(-2.0f, -1.0f , -1.0f) ); //tl
	bottomPlane.addVertex( ofVec3f(+2.0f, -1.0f , -1.0f) ); //tr
	bottomPlane.addVertex( ofVec3f(-2.0f, -1.0f , +1.0f) ); //bl
	bottomPlane.addVertex( ofVec3f(+2.0f, -1.0f , +1.0f) ); //br
	bottomPlane.addTexCoords(vector<ofVec2f>(4));
	bottomPlane.addTriangle(0, 1, 2);
	bottomPlane.addTriangle(1, 3, 2);

	backPlane.addVertex( ofVec3f(-3.0f, 3.0f, -1.5f) );
	backPlane.addVertex( ofVec3f(+3.0f, 3.0f, -1.5f) );
	backPlane.addVertex( ofVec3f(-3.0f, -1.2f, -1.5f) );
	backPlane.addVertex( ofVec3f(+3.0f, -1.2f, -1.5f) );
	backPlane.addVertex( ofVec3f(-3.0f, -1.2f, +1.5f) );
	backPlane.addVertex( ofVec3f(+3.0f, -1.2f, +1.5f) );
	backPlane.addTriangle(0, 1, 2);
	backPlane.addTriangle(1, 3, 2);
	backPlane.addTriangle(2, 3, 4);
	backPlane.addTriangle(4, 3, 5);
}

//---------
void Positions::customDraw() {
	this->createReflection();
	this->drawMesh();
	this->drawReflection();
	this->drawBack();
}

//---------
void Positions::createReflection() {
	this->reflected.begin(false);
	ofClear(0.0f, 255.0f);
	ofSetColor(255.0f);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(0.0f, -1.0f, 0.0f);
	this->drawMesh();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	this->reflected.end();
}

//---------
void Positions::drawMesh() {
	if (this->map.isAllocated() && this->map.isUsingTexture()) {
		this->map.bind();
		this->mesh.draw();
		this->map.unbind();
	} else
		this->mesh.draw();
}

//---------
void Positions::setTexCd(int index, const ofMatrix4x4 & viewProj) {
	ofVec4f texcd;
	texcd = this->bottomPlane.getVertex(index) * viewProj;
	texcd /= texcd.w;
	texcd.y *= -1;
	texcd += ofVec2f(1.0f, 1.0f);
	texcd *= ofVec2f(LAYER_WIDTH / 2.0f, LAYER_HEIGHT / 2.0f);
	this->bottomPlane.setTexCoord(index, texcd);
	cout << index << " = " << texcd << endl;
}

//---------
void Positions::drawReflection() {
	//fuck it, just draw without 
	ofPushMatrix();
	ofTranslate(0.0f, -1.0f, 0.0f);
	ofRotate(90.0f, 1.0f, 0.0f, 0.0f);
	ofPushStyle();
	ofSetColor(100);
	this->reflected.draw(-2,1, 4, -2);
	ofPopStyle();
	ofPopMatrix();
	/*
	ofMatrix4x4 proj, view;
	glGetFloatv( GL_PROJECTION_MATRIX, proj.getPtr() );
	glGetFloatv( GL_MODELVIEW_MATRIX, view.getPtr() );
	view.postMult(proj);

	for (int i=0; i<4; i++)
		setTexCd(i, view);

	ofPushMatrix();
	this->reflected.getTextureReference().bind();
	this->bottomPlane.draw();
	this->reflected.getTextureReference().unbind();
	ofPopMatrix();
	*/
}

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
#define WIDTH 0.02f
#define HEIGHT 0.05f
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
void Positions::drawBack() {
	ofPushStyle();
	ofSetColor(30.0f);
	this->backPlane.draw();
	ofPopStyle();
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