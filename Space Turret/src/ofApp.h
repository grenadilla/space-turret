#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

#include "planet.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "powerup.h"

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

		void contactStart(ofxBox2dContactArgs &e);
        void contactEnd(ofxBox2dContactArgs &e);
		
	private:
		ofSoundPlayer background_music;

		ofTrueTypeFont font;

		ofImage blast;
        ofImage ion;

		ofxBox2d box2d;

		shared_ptr<Planet> fuel_planet;
        shared_ptr<Planet> ammo_planet;
        shared_ptr<Player> player_ship;

		std::vector<std::shared_ptr<Bullet>> bullets;
        int bullet_index;

		std::vector<std::shared_ptr<Enemy>> enemies;
        int enemy_index;

		std::vector<std::shared_ptr<Powerup>> powerups;
        int powerup_index;

		std::set<int> keys_pressed;

		int difficulty_increase_timer;
        std::vector<double> spawn_rates;

		void removeBullets();
        void removeEnemies();
};
