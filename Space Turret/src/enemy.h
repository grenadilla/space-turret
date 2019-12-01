#pragma once

#include "identifier.h"
#include "preloaded.h"

#include "ofMain.h"
#include "ofxBox2d.h"

class Enemy : public ofxBox2dPolygon, public Preloaded {
   public:
	Enemy();
    Enemy(b2World *b2World, int size, float density = 1);
    void Attack(int x, int y, int target_x, int target_y, int speed, int health = 1);
    void Damage(int damage = 1);
    void Reset();
    void Retarget();

   private:
    int health;
    int target_x;
    int target_y;
    int speed;

	b2Vec2 GetVelocity();
    int GetAngle();
};