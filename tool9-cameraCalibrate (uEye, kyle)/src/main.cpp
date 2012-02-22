#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1200, 900, OF_WINDOW);
	ofRunApp(new testApp());
}
