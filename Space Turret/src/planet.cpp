#include "planet.h"

#include "ofMain.h"

constexpr int hex_color = 0x90d4e3;

Planet::Planet() = default;

Planet::Planet(b2World *b2dWorld, int x, int y, int radius, float density,
               float bounce, float friction) {
    setPhysics(density, bounce, friction);
    setup(b2dWorld, x, y, radius);
    body->SetType(b2_staticBody);
}

SpaceType Planet::GetType() { return SpaceType::Planet; }

