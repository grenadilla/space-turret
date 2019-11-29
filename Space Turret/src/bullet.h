#pragma once

#include "ofxBox2d.h"
#include "identifier.h"
#include "preloaded.h"

class Bullet : public ofxBox2dRect, public virtual Preloaded {
   public:
    Bullet();
    Bullet(b2World *b2World, int height, int width, float density = 1);
    void Shoot(int x, int y, int player_ship_radius, float rotation,
               int bullet_speed);
    void Reset();
};