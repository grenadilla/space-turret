#pragma once

#include "ofxBox2d.h"
#include "ofMain.h"
#include "preloaded.h"

#include <map>
#include <string>

class Powerup : public ofxBox2dCircle, public Preloaded {
   public:
    enum class Type {
        Damage, Spray, Health, Ammo, Fuel,
    };

    static constexpr int health_restore = 1;
    static constexpr int fuel_cap_increase = 250;
    static constexpr int ammo_cap_increase = 5;

    Powerup();
    Powerup(b2World *b2dWorld, int radius, float density = 1,
            float bounce = 0, float friction = 0);
    void Prepare(int x, int y, Type type, int color);
    void Drop();
    static void LoadImages(std::map<Powerup::Type, ofImage> images);
    void Reset();
    void draw();

    Type GetType();

  private:
    static std::map<Powerup::Type, ofImage> images;

    int x;
    int y;
    int color;
    Type type;
};