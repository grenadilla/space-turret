#pragma once

#include "ofxBox2d.h"

class Identifier {
   public:
    enum class ShapeType {
        Player,
        Planet,
        Bullet,
        Enemy,
        Powerup,
    };

    Identifier(ShapeType type, ofxBox2dBaseShape *shape);
    ShapeType GetType();
    shared_ptr<ofxBox2dBaseShape> GetShape();

    static const int player_category = 0x0002;
    static const int enemy_category = 0x0004;
    static const int planet_category = 0x0008;

   private:
    ShapeType type;
    shared_ptr<ofxBox2dBaseShape> shape;
};