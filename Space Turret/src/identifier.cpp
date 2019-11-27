#include "identifier.h"

Identifier::Identifier(ShapeType type, ofxBox2dBaseShape *shape)
	: type(type), shape(shape) {}

Identifier::ShapeType Identifier::GetType() { return type; }

shared_ptr<ofxBox2dBaseShape> Identifier::GetShape() { return shape; }