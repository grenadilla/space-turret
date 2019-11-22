#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include <cmath>
#include <map>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofSoundPlayer background_music;

		ofxBox2d box2d;
		ofxBox2dCircle fuel_planet;
        ofxBox2dCircle ammo_planet;
        ofxBox2dCircle player_ship;

		std::map<int, bool> keys_pressed;
};
