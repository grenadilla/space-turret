#include "planet.h"

#include "identifier.h"
#include "ofMain.h"

constexpr int hex_color = 0x90d4e3;

Planet::Planet() = default;

Planet::Planet(b2World *b2dWorld, int x, int y, int radius, float density,
               float bounce, float friction) {
    setPhysics(density, bounce, friction);
    setup(b2dWorld, x, y, radius);
    body->SetType(b2_staticBody);
    body->SetUserData(new Identifier(Identifier::ShapeType::Planet, this));
}

