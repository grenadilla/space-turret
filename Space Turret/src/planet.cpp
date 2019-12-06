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

    b2Filter filter;
    filter.categoryBits = Identifier::planet_category;
    setFilterData(filter);

    setData(new Identifier(Identifier::ShapeType::Planet, this));
}

bool Planet::IsTouchingPlayer() { return touching_player; }

void Planet::SetTouchingPlayer(bool touching) { touching_player = touching; }