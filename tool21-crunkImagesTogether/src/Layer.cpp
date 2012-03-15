#include "Layer.h"
Layer::Layer(const Positions & positions, int index) :
	positions(positions) {
	this->index = index;
	this->pixels.allocate(LAYER_WIDTH, LAYER_HEIGHT, OF_IMAGE_COLOR_ALPHA);
	this->allocate(this->pixels);
}

//---------
void Layer::load(string path) {
	fileIndices.clear();
	dir.allowExt("png");
	dir.listDir(path);
	for (int i=0; i < dir.size(); i++) {
		fileIndices.push_back(i);
	}

#pragma omp critical(ofLog)
	cout << "Layer " << ofFilePath::getFileName(path) << " loaded " << fileIndices.size() << " images." << endl;

	this->setFrame(0);
	scaleInLayer = ofVec2f(this->getWidth() / 4.0f * 0.02f, this->getHeight() / 2.0f * 0.05f);
	this->createMesh(positions);
}

//---------
void Layer::setFrame(int frame) {
	ofFile & file (dir.getFile(fileIndices[frame]));
	if ( file.is_open() )
		ofLoadImage(this->pixels, file);
	else
		ofLoadImage( this->pixels, file.getAbsolutePath() );
}

//---------
void Layer::update() {
	this->loadData(pixels);
}

//---------
int Layer::count() const {
	return fileIndices.size();
}

void tl(ofMesh & mesh, const ofVec2f & tl, const ofRectangle & bounds) {
	mesh.addVertex(ofVec3f(bounds.x, bounds.y, 0));
	mesh.addTexCoord( ofVec2f(tl.x, tl.y) );
	//mesh.addColor(ofFloatColor(tl.x / 1024.0f, tl.y / 512.0f, 0, 1.0f) );
}

void tr(ofMesh & mesh, const ofVec2f & tl, const ofRectangle & bounds) {
	mesh.addVertex(ofVec3f(bounds.x + bounds.width, bounds.y, 0));
	mesh.addTexCoord( ofVec2f(tl.x + scaleInLayer.x, tl.y) );
	//mesh.addColor(ofFloatColor(tl.x / 1024.0f, tl.y / 512.0f, 0, 1.0f) );
}

void bl(ofMesh & mesh, const ofVec2f & tl, const ofRectangle & bounds) {
	mesh.addVertex(ofVec3f(bounds.x, bounds.y + bounds.height, 0));
	mesh.addTexCoord( ofVec2f(tl.x, tl.y + scaleInLayer.y) );
	//mesh.addColor(ofFloatColor(tl.x / 1024.0f, tl.y / 512.0f, 0, 1.0f) );
}

void br(ofMesh & mesh, const ofVec2f & tl, const ofRectangle & bounds) {
	mesh.addVertex(ofVec3f(bounds.x + bounds.width, bounds.y + bounds.height, 0));
	mesh.addTexCoord( ofVec2f(tl.x + scaleInLayer.x, tl.y + scaleInLayer.y) );
	//mesh.addColor(ofFloatColor(tl.x / 1024.0f, tl.y / 512.0f, 0, 1.0f) );
}

//---------
void Layer::createMesh(const Positions & positions) {
	ofRectangle bounds;
	ofVec3f position;
	ofVec2f texCd;
	int layer;
	for (int i=0; i<positions.size(); i++) {
		position = positions[i];
		layer = ofMap(position.z, -1 , 1 - 0.125, 0, 15, true);

		if (layer == this->index) {
			texCd.x = ofMap(position.x, -2, 2, 0, (this->getWidth() - FACE_RES_X), true);
			texCd.y = ofMap(-position.y, -1, 1, 0, (this->getHeight() - FACE_RES_Y), true);
			bounds = positions.subImageBounds[i];

			tl(mesh, texCd, bounds);
			tr(mesh, texCd, bounds);
			bl(mesh, texCd, bounds);

			tr(mesh, texCd, bounds);
			br(mesh, texCd, bounds);
			bl(mesh, texCd, bounds);
		}
	}
}

//---------
void Layer::drawBits() {
	ofSetColor(255);
	this->update();
	this->bind();
	this->mesh.draw();
	this->unbind();
}