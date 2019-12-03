#pragma once

#include "ofxBox2d.h"

class Player : public ofxBox2dCircle {
   public:
    Player();
    Player(b2World *b2dWorld, int x, int y, int radius, 
		int health, int fuel, int ammo,
		float density = 0, float bounce = 0, float friction = 0.1);

	int GetMaxHealth();
    void SetMaxHealth(int max_health);
    int GetHealth();
    void SetHealth(int health);
    int GetMaxFuel();
    void SetMaxFuel(int max_fuel);
    int GetFuel();
    void SetFuel(int fuel);
    int GetMaxAmmo();
    void SetMaxAmmo(int max_ammo);
    int GetAmmo();
    void SetAmmo(int ammo);

   private:
    int max_health;
    int health;
    int max_fuel;
    int fuel;
    int max_ammo;
    int ammo;
};