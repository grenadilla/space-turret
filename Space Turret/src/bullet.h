#pragma once

#include "ofxBox2d.h"
#include "identifier.h"

class Bullet : public ofxBox2dRect {
   public:
    Bullet();
    Bullet(b2World *b2World, int height, int width, float density = 1);
    void Shoot(int x, int y, int player_ship_radius, float rotation,
               int bullet_speed);
    void Reset();

    bool DidCollide();
    void SetCollided(bool collided);
    bool IsInUse();

	private:
		bool collided;
        bool in_use;
};