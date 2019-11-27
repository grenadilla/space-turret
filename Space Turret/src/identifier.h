#pragma once

#include "ofxBox2d.h"

class Identifier {

   public:
    enum class ShapeType {
		Player,
        Planet,
        Bullet,
    };

        Identifier(ShapeType type, ofxBox2dBaseShape *shape);
		ShapeType GetType();
        shared_ptr<ofxBox2dBaseShape> GetShape();

   private:
        ShapeType type;
		shared_ptr<ofxBox2dBaseShape> shape;
};