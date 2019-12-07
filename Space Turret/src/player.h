#pragma once

#include "ofxBox2d.h"
#include "powerup.h"

class Player : public ofxBox2dCircle {
   public:
    Player();
    Player(b2World *b2dWorld, int x, int y, int radius, 
		int health, int fuel, int ammo, int fuel_refresh, int ammo_refresh,
		int attack = 1, float density = 1, float bounce = 0, float friction = 0);

    void Upgrade(Powerup::Type type);

    static constexpr int max_spray = 3;

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
    int GetFuelRefresh();
    void SetFuelRefresh(int fuel_refresh);
    int GetAmmoRefresh();
    void SetAmmoRefresh(int ammo_refresh);
    int GetAttack();
    void SetAttack(int attack);
    int GetSpray();
    void SetSpray(int spray);

   private:
    int max_health;
    int health;
    int max_fuel;
    int fuel;
    int max_ammo;
    int ammo;
    int fuel_refresh;
    int ammo_refresh;
    int attack;
    int spray;
};