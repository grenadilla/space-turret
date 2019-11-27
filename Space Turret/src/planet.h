#pragma once
#include "ofxBox2d.h"
#include "spaceType.h"

class Planet : public ofxBox2dCircle {
   public:
    Planet();
    Planet(b2World *b2dWorld, int x, int y, int radius, 
		float density = 0, float bounce = 0, float friction = 0.1);
    SpaceType GetType();
};
