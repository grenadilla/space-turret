#include "bullet.h"

#include "identifier.h"
#include "ofMain.h"

#include <iostream>

const double kDegreeRadMult = PI / 180;
constexpr int x_hiding = -10;
constexpr int y_hiding = -10;
constexpr int rotation_hiding = 0;

Bullet::Bullet() = default;

Bullet::Bullet(b2World *b2World, int height, int width, float density) {
    collided = false;

	//Create rectangular outline of bullet
    ofRectangle bullet_rect(x_hiding, y_hiding, height, width);

	// Bullets need to have mass or else they are treated like static unmovable
    // objects
	setPhysics(density, 0, 0);
    setup(b2World, bullet_rect, rotation_hiding);
    body->SetBullet(true);
    body->SetActive(false);

	setData(new Identifier(Identifier::ShapeType::Bullet, this));
}

void Bullet::Shoot(int x, int y, int player_ship_radius, float rotation,
	int bullet_speed) {
    setPosition(x + std::cos(rotation * kDegreeRadMult) * player_ship_radius,
                y + std::sin(rotation * kDegreeRadMult) * player_ship_radius);

	setRotation(rotation);

    // Set bullet velocity in direction of player rotation
    b2Vec2 bullet_velocity;
    bullet_velocity.Set(std::cos(rotation * kDegreeRadMult) * bullet_speed,
                        std::sin(rotation * kDegreeRadMult) * bullet_speed);
    body->SetLinearVelocity(bullet_velocity);
    in_use = true;
    body->SetActive(true);
}

void Bullet::Reset() { 
	in_use = false;
    collided = false;
    body->SetLinearVelocity({0, 0});
    setPosition(x_hiding, y_hiding);
    body->SetActive(false);
}

void Bullet::SetCollided(bool collided) { this->collided = collided; }

bool Bullet::DidCollide() { return collided; }

bool Bullet::IsInUse() { return in_use; }
