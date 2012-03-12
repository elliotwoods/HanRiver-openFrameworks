#include "SimpleConfig.h"

void SimpleConfig::init(string filename) {
	file.open(filename, ofFile::ReadWrite, false);
}

string SimpleConfig::getFilename(string attributeName) {
	file.seekg(0);
	
	char attribute[512];
	char value[512];
	memset(attribute, '\0', 512);
	memset(value, '\0', 512);

	try {
		while (string(attribute) != attributeName) {
			file.getline(attribute, 512);
			file.getline(value, 512);
		}

		if (!ofFile::doesFileExist(string(value)))

	} catch (...) {
		return addFilename(attributeName);
	}
}

string SimpleConfig::addFilename(string attributeName) {
	file.seekp(file.end);
	string filename = ofSystemLoadDialog(attributeName).getPath();
	file << endl << attributeName;
	file << endl << filename;
	return filename;
}

string SimpleConfig::changeFilename(string attributeName) {
	char attribute[512];
	char value[512];
	int valuePosition = 0;

	file.seekg(0);
	try {
		while (!file.eof()) {
			file.getline(attribute, 512);
			if (string(attribute) == attributeName)
				break;
			file.getline(value, 512);
		}
		file.
	} catch (...) {
		return addFilename(attributeName);
	}
}