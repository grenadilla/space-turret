#include "enemy.h"

#include <iostream>

const double kDegreeRadMult = PI / 180;
constexpr int kDegreeInCircle = 360;

constexpr int x_hiding = -20;
constexpr int y_hiding = -20;

constexpr int tri_angle = 30;

Enemy::Enemy() = default;

Enemy::Enemy(b2World* b2World, int size, float density) {
    addVertex(glm::vec3(x_hiding, y_hiding, 0));
    addVertex(glm::vec3(
        x_hiding + size * std::cos(-1 * tri_angle * kDegreeRadMult),
        y_hiding + size * std::sin(-1 * tri_angle * kDegreeRadMult), 0));
    addVertex(glm::vec3(x_hiding + size * std::cos(tri_angle * kDegreeRadMult),
                        y_hiding + size * std::sin(tri_angle * kDegreeRadMult),
                        0));

    setPhysics(density, 0, 0);
    triangulate();
    create(b2World);

    setData(new Identifier(Identifier::ShapeType::Enemy, this));

    collided = false;
    body->SetActive(false);
}

b2Vec2 Enemy::GetVelocity() {
    b2Vec2 velocity(target_x - getPosition().x, target_y - getPosition().y);
    velocity.Normalize();
    velocity *= speed;
    return velocity;
}

int Enemy::GetAngle() {
    // Get angle in radians, convert to degrees, and set to between 0 and 360
    int angle = std::atan((double)(target_y - getPosition().y) / (target_x - getPosition().x));
    angle /= kDegreeRadMult;
    angle = (angle + kDegreeInCircle) % kDegreeInCircle;
    return angle;
}

void Enemy::Retarget() {
    setRotation(GetAngle());
    body->SetLinearVelocity(GetVelocity());
    body->SetAngularVelocity(0);
}

void Enemy::Attack(int x, int y, int target_x, int target_y, int speed, int health) {
    this->health = health;
    this->target_x = target_x;
    this->target_y = target_y;
    this->speed = speed;

    setPosition(x, y);
    Retarget();

    in_use = true;
    body->SetActive(true);
}

void Enemy::Damage(int damage) { 
	health -= damage;
    if (health <= 0) {
        collided = true;
	}
}

void Enemy::Reset() {
    in_use = false;
    collided = false;
    body->SetActive(false);
    body->SetLinearVelocity({0, 0});
    body->SetAngularVelocity(0);
    setPosition(x_hiding, y_hiding);
}