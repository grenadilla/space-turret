#include "bullet.h"

#include "identifier.h"
#include "calculations.h"
#include "ofMain.h"

#include <iostream>

//Offscreen location where unused bullets should hide
constexpr int x_hiding = -10;
constexpr int y_hiding = -10;

Bullet::Bullet() = default;

Bullet::Bullet(b2World *b2World, int height, int width, float density) {
	//Create rectangular outline of bullet
    ofRectangle bullet_rect(x_hiding, y_hiding, height, width);

	// Bullets need to have mass or else they are treated like static unmovable
    // objects
	setPhysics(density, 0, 0);
    setup(b2World, bullet_rect, 0);
    body->SetBullet(true);

    b2Filter filter;
    filter.maskBits = Identifier::enemy_category | Identifier::planet_category;
    setFilterData(filter);

	setData(new Identifier(Identifier::ShapeType::Bullet, this));

	collided = false;
    body->SetActive(false);
    in_use = false;
}

void Bullet::Shoot(int x, int y, int player_ship_radius, float rotation,
	int bullet_speed, int offset_angle) {
    setPosition(x + std::cos((rotation + offset_angle) * calc::kDegreeRadMult) * player_ship_radius,
                y + std::sin((rotation + offset_angle) * calc::kDegreeRadMult) * player_ship_radius);

	setRotation(rotation);

    // Set bullet velocity in direction of player rotation
    b2Vec2 bullet_velocity;
    bullet_velocity.Set(std::cos(rotation * calc::kDegreeRadMult) * bullet_speed,
                        std::sin(rotation * calc::kDegreeRadMult) * bullet_speed);
    body->SetLinearVelocity(bullet_velocity);

	in_use = true;
    body->SetActive(true);
}

void Bullet::Reset() { 
	in_use = false;
    collided = false;
    body->SetActive(false);
    body->SetLinearVelocity({0, 0});
    body->SetAngularVelocity(0);
    setPosition(x_hiding, y_hiding);
}
