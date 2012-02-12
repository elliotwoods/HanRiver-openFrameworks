#pragma once
#include "ofMain.h"

class SimpleConfig {
public:
	void init(string filename);
	string getFilename(string attributeName);
protected:
	string addFilename(string attributeName);
	string changeFilename(string attributeName);
	ofFile file;
};