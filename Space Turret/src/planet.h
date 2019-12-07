#pragma once
#include "ofxBox2d.h"
#include "ofMain.h"

class Planet : public ofxBox2dCircle {
   public:
    Planet();
    Planet(b2World *b2dWorld, int x, int y, int radius, int color,
           ofImage *image = nullptr, float density = 0, float bounce = 0, float friction = 0.1);
    void draw();

	bool IsTouchingPlayer();
    void SetTouchingPlayer(bool touching);

   private:
    bool touching_player;
    int color;
    ofImage *image;
};
