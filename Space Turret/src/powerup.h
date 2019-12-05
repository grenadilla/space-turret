#pragma once

#include "ofxBox2d.h"
#include "ofMain.h"
#include "preloaded.h"

#include <map>
#include <string>

class Powerup : public ofxBox2dCircle, public Preloaded {
   public:
    enum class Type {
        Damage, Spray,
    };
    Powerup();
    Powerup(b2World *b2dWorld, int radius, float density = 1,
            float bounce = 0, float friction = 0);
    void Drop(int x, int y, Type type, int color);
    static void LoadImages(std::map<Powerup::Type, ofImage> images);
    void Reset();
    void draw();

  private:
    static std::map<Powerup::Type, ofImage> images;

    int color;
    Type type;
};