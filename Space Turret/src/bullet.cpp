#include "bullet.h"

#include "ofMain.h"

const double kDegreeRadMult = PI / 180;

Bullet::Bullet() = default;

Bullet::Bullet(b2World *b2World, int x, int y, int height, int width, int player_ship_radius, float rotation,
	int bullet_speed, float density) {

	//Create rectangular outline of bullet
    ofRectangle bullet_rect(
        x + std::cos(rotation * kDegreeRadMult) * player_ship_radius,
        y + std::sin(rotation * kDegreeRadMult) * player_ship_radius,
        height, width);

	// Bullets need to have mass or else they are treated like static unmovable
    // objects
	setPhysics(density, 0, 0);
    setup(b2World, bullet_rect, rotation);
    body->SetBullet(true);

	//Set bullet velocity in direction of player rotation
	b2Vec2 bullet_velocity;
    bullet_velocity.Set(
        std::cos(rotation * kDegreeRadMult) * bullet_speed,
        std::sin(rotation * kDegreeRadMult) * bullet_speed);
    body->SetLinearVelocity(bullet_velocity);
}