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

void Enemy::Attack(int x, int y, int target_x, int target_y, int speed) {
    setPosition(x, y);

    // Get angle in radians, convert to degrees, and set to between 0 and 360
    int angle = std::atan((double)(target_y - y) / (target_x - x));
    angle /= kDegreeRadMult;
    angle = (angle + kDegreeInCircle) % kDegreeInCircle;

    setRotation(angle);

    b2Vec2 velocity(target_x - x, target_y - y);
    velocity.Normalize();

    std::cout << velocity.x << ' ' << velocity.y << std::endl;

    velocity *= speed;

    body->SetLinearVelocity(velocity);

    in_use = true;
    body->SetActive(true);
}

// Note - maybe convert to interface later

void Enemy::Reset() {
    in_use = false;
    collided = false;
    body->SetActive(false);
    body->SetLinearVelocity({0, 0});
    setPosition(x_hiding, y_hiding);
}

void Enemy::SetCollided(bool collided) { this->collided = collided; }

bool Enemy::DidCollide() { return collided; }

bool Enemy::IsInUse() { return in_use; }
