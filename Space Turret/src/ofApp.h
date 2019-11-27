#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "planet.h"
#include <cmath>
#include <set>
#include <vector>
#include <memory>

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
		
	private:
		ofSoundPlayer background_music;

		ofxBox2d box2d;

		shared_ptr<Planet> fuel_planet;
        shared_ptr<Planet> ammo_planet;

        ofxBox2dCircle player_ship;

		std::vector<std::shared_ptr<ofxBox2dRect>> bullets;

		std::set<int> keys_pressed;

		void removeBullets();
        void shootBullet();
};
