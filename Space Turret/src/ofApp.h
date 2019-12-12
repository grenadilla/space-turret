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
#include <string>

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
        void Preload();
        void SpawnEnemy();
        void SpawnPowerup(int x, int y);
        void UpdateSpawnRate();
        void ShootBullet();
        void MovePlayer();
        void AddGravity();
        void WrapAroundPlayer();
        void SetPlanetContact();
        void RestockPlayer();

		void contactStart(ofxBox2dContactArgs &e);
        void contactEnd(ofxBox2dContactArgs &e);

		ofxBox2d box2d;

		bool game_over;

		// Music and sound
		ofSoundPlayer background_music;
        ofSoundPlayer laser_sound;
        ofSoundPlayer damage_sound;
        ofSoundPlayer powerup_sound;
        ofSoundPlayer explosion_sound;

		ofTrueTypeFont font;

		// Powerup images
		ofImage blast;
        ofImage ion;
        ofImage heart_plus;
        ofImage oil_drum;
        ofImage heavy_bullets;

		// Planet images
        ofImage fuel_icon;
        ofImage ammo_icon;

		std::string powerup_message;
        int powerup_message_timer;

		shared_ptr<Planet> fuel_planet;
        shared_ptr<Planet> ammo_planet;
        shared_ptr<Player> player_ship;

		std::vector<std::shared_ptr<Bullet>> bullets;
        int bullet_index;

		std::vector<std::shared_ptr<Enemy>> enemies;
        int enemy_index;

		std::vector<std::shared_ptr<Powerup>> powerups;
        int powerup_index;
        shared_ptr<Powerup> powerup_to_drop = nullptr;

		std::set<int> keys_pressed;

		int difficulty_increase_timer;
        std::vector<double> spawn_rates;

		int bullet_timer;
        int reload_timer;

		void removePreloadedObjects();
};
