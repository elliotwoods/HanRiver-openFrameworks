#include "ThreadedCapture.h"

ThreadedCapture::ThreadedCapture() {
	this->ready = false;
	this->frameNew = false;
	this->hasNewFrame = false;
}

void ThreadedCapture::update() {
	this->frameNew = this->hasNewFrame;
	this->hasNewFrame = false;
}

void ThreadedCapture::draw(int x, int y) {
	this->draw(x, y, secondBuffer.getWidth(), secondBuffer.getHeight());
}

void ThreadedCapture::draw(int x, int y, int width, int height) {
	if (!this->secondBuffer.isAllocated())
		return;
	lock();
	secondBuffer.update();
	secondBuffer.draw(x, y, width, height);
	unlock();
}

void ThreadedCapture::open(ofxUeyeDevice& device) {	
	if (this->isThreadRunning())
		this->stopThread();

	this->device = device;
	this->startThread(true, false);
}

void ThreadedCapture::close() {
	this->stopThread();
}

ofPixels& ThreadedCapture::getPixelsRef() {
	return this->secondBuffer.getPixelsRef();
}

unsigned char* ThreadedCapture::getPixels() {
	return this->secondBuffer.getPixels();
}

int ThreadedCapture::getWidth() {
	return this->camera.getWidth();
}

int ThreadedCapture::getHeight() {
	return this->camera.getHeight();
}

ofxUeye& ThreadedCapture::getCamera() {
	return this->camera;
}

bool ThreadedCapture::isReady() const{
	return this->ready;
}

bool ThreadedCapture::isFrameNew() const {
	return this->frameNew;
}

void ThreadedCapture::threadedFunction() {
	camera.init(device);
	camera.setPixelClock(PIXELCLOCK);
	this->ready = true;
	while (this->isThreadRunning()) {
		camera.update();
		lock();
		secondBuffer = camera.getPixelsRef();
		this->hasNewFrame = true;
		unlock();
	}
	lock();
	this->ready = false;
	camera.close();
	unlock();
}