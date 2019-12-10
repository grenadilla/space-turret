#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setSize(1024, 768);  // <---- Set the window dimensions.
    settings.resizable = false;

    shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
    shared_ptr<ofApp> app(new ofApp());

    ofRunApp(window, app);
    ofRunMainLoop();
}
