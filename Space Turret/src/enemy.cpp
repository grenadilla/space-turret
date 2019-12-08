#include "enemy.h"
#include "calculations.h"

#include <iostream>

constexpr double kDegreeRadMult = PI / 180;

constexpr int x_hiding = -20;
constexpr int y_hiding = -20;

constexpr int tri_angle = 30;

std::vector<int> Enemy::colors;

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

    b2Filter filter;
    filter.categoryBits = Identifier::enemy_category;
    setFilterData(filter);

    setData(new Identifier(Identifier::ShapeType::Enemy, this));

    collided = false;
    body->SetActive(false);
    in_use = false;
}

b2Vec2 Enemy::GetVelocity() {
    b2Vec2 velocity(target_x - getPosition().x, target_y - getPosition().y);
    velocity.Normalize();
    velocity *= speed;
    return velocity;
}

void Enemy::Retarget() {
	//+180 to flip sharp point
    setRotation(calc::GetAngle(target_x - getPosition().x, target_y - getPosition().y) + 180);
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

bool Enemy::Damage(int damage) { 
    // Return whether the enemy was destroyed
	health -= damage;
    if (health <= 0) {
        collided = true;
        return true;
	}

    return false;
}

void Enemy::Reset() {
    in_use = false;
    collided = false;
    body->SetActive(false);
    body->SetLinearVelocity({0, 0});
    body->SetAngularVelocity(0);
    setPosition(x_hiding, y_hiding);
}

void Enemy::SetColors(std::vector<int> colors) { Enemy::colors = colors; }

//Create out own version of the draw method, named the same as box2d draw
//Must call SetColors sometime before in setup
void Enemy::draw() {
    int color;
    if (health > colors.size()) {
        color = colors.back();
    } else {
        color = colors[health - 1];
	}

	ofFill();
    ofSetHexColor(color);
    ofxBox2dPolygon::draw();
}