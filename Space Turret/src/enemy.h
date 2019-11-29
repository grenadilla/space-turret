#pragma once

#include "identifier.h"
#include "preloaded.h"

#include "ofMain.h"
#include "ofxBox2d.h"

class Enemy : public ofxBox2dPolygon, public Preloaded {
   public:
	Enemy();
    Enemy(b2World *b2World, int size, float density = 1);
    void Attack(int x, int y, int target_x, int target_y, int speed);
    void Reset();
};