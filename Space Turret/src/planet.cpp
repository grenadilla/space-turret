#include "planet.h"

#include <iostream>
#include "ofMain.h"

constexpr int hex_color = 0x90d4e3;

Planet::Planet() = default;

Planet::Planet(b2World *b2dWorld, int x, int y, int radius, float density,
               float bounce, float friction) {
    std::cout << x << ' ' << y << std::endl;
    setPhysics(density, bounce, friction);
    setup(b2dWorld, x, y, radius);
    std::cout << getPosition().x << ' ' << getPosition().y << std::endl;
    body->SetType(b2_staticBody);
    std::cout << density << std::endl;
}

