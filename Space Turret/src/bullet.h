#pragma once

#include "ofxBox2d.h"

class Bullet : public ofxBox2dRect {
   public:
    Bullet();
    Bullet(b2World *b2World, int x, int y, int height, int width,
           int player_ship_radius, float rotation, int bullet_speed, float density = 1);
};