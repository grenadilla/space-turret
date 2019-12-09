#include "planet.h"

#include "identifier.h"
#include "ofMain.h"
#include "calculations.h"

constexpr int hex_color = 0x90d4e3;

Planet::Planet() = default;

Planet::Planet(b2World *b2dWorld, int x, int y, int radius, int color,
               ofImage *image, float density, float bounce, float friction) {
    setPhysics(density, bounce, friction);
    setup(b2dWorld, x, y, radius);
    body->SetType(b2_staticBody);

    b2Filter filter;
    filter.categoryBits = Identifier::planet_category;
    setFilterData(filter);

    this->touching_player = false;
    this->color = color;
    this->image = image;

    setData(new Identifier(Identifier::ShapeType::Planet, this));
}

void Planet::draw() {
    ofFill();
    ofSetHexColor(color);
    ofDrawCircle(getPosition().x, getPosition().y, getRadius());

    if (image != nullptr) {
        int square_side_length = getRadius() * 2 / std::sqrt(2);
        image->draw(getPosition().x + getRadius() * std::cos(calc::kLeftCornerDegree *
                                                             calc::kDegreeRadMult),
                    getPosition().y + getRadius() * std::sin(calc::kLeftCornerDegree *
                                                             calc::kDegreeRadMult),
                    square_side_length, square_side_length);
    }
}

bool Planet::IsTouchingPlayer() { return touching_player; }

void Planet::SetTouchingPlayer(bool touching) { touching_player = touching; }