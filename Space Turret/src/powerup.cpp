#include "powerup.h"

#include "identifier.h"
#include "calculations.h"

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
    in_use = false;

    //Set collision categories and filter
    b2Filter filter;
    filter.maskBits = Identifier::player_category;
    setFilterData(filter);
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

void Powerup::Prepare(int x, int y, Type type, int color) {
    this->color = color;
    this->type = type;
    this->x = x;
    this->y = y;
}

void Powerup::Drop() {
    body->SetActive(true);
    setPosition(x, y);
    in_use = true;
}

void Powerup::draw() {
    ofFill();
    ofSetHexColor(color);
    ofDrawCircle(getPosition().x, getPosition().y, getRadius());

    // Draw powerup icon. All powerups have an icon, unlike planets which don't need one
    int square_side_length = getRadius() * 2 / std::sqrt(2);
    Powerup::images[type].draw(
        getPosition().x +
            getRadius() * std::cos(calc::kLeftCornerDegree * calc::kDegreeRadMult),
        getPosition().y +
            getRadius() * std::sin(calc::kLeftCornerDegree * calc::kDegreeRadMult),
        square_side_length, square_side_length);
}

Powerup::Type Powerup::GetType() { return type; }