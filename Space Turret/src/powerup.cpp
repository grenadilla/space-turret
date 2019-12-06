#include "powerup.h"

#include "identifier.h"

const double kDegreeRadMult = PI / 180;
const int kLeftCornerDegree = 225;

constexpr int x_hiding = -20;
constexpr int y_hiding = -40;

std::map<Powerup::Type, ofImage> Powerup::images;

Powerup::Powerup() = default;

Powerup::Powerup(b2World* b2dWorld, int radius, float density, float bounce,
                 float friction) {
    setPhysics(density, bounce, friction);
    setup(b2dWorld, x_hiding, y_hiding, radius);
    setData(new Identifier(Identifier::ShapeType::Powerup, this));

    collided = false;
    body->SetActive(false);

    //Set collision categories and filter
    b2Filter filter;
    filter.maskBits = Identifier::player_category;
    setFilterData(filter);

    // Use below to prevent collision between powerups and enemies
    // fixture.filter.groupIndex =
}

void Powerup::LoadImages(std::map<Powerup::Type, ofImage> images) {
    Powerup::images = images;
}

void Powerup::Reset() {
    in_use = false;
    collided = false;
    body->SetActive(false);
    body->SetLinearVelocity({0, 0});
    body->SetAngularVelocity(0);
    setPosition(x_hiding, y_hiding);
}

void Powerup::Drop(int x, int y, Type type, int color) {
    this->color = color;
    this->type = type;

    setPosition(x, y);
    body->SetActive(true);
}

void Powerup::draw() {
    ofFill();
    ofSetHexColor(color);
    ofDrawCircle(getPosition().x, getPosition().y, getRadius());

    int square_side_length = getRadius() * 2 / std::sqrt(2);
    Powerup::images[type].draw(
        getPosition().x +
            getRadius() * std::cos(kLeftCornerDegree * kDegreeRadMult),
        getPosition().y +
            getRadius() * std::sin(kLeftCornerDegree * kDegreeRadMult),
        square_side_length, square_side_length);
}