#include "player.h";

#include "identifier.h"
#include "ofMain.h"

Player::Player() = default;

Player::Player(b2World* b2dWorld, int x, int y, int radius, int health,
               int fuel, int ammo, int fuel_refresh, int ammo_refresh,
               int attack, float density, float bounce, float friction) {
    setPhysics(density, bounce, friction);
    setup(b2dWorld, x, y, radius);
    // Set fixed rotation so player ship doesn't roll around with friction
    setFixedRotation(true);

    max_health = health;
    max_fuel = fuel;
    max_ammo = ammo;
    this->health = health;
    this->fuel = fuel;
    this->ammo = ammo;
    this->fuel_refresh = fuel_refresh;
    this->ammo_refresh = ammo_refresh;
    this->attack = attack;
    spray = 1;

    // Set collision filters
    b2Filter filter;
    filter.categoryBits = Identifier::player_category;
    setFilterData(filter);

    setData(new Identifier(Identifier::ShapeType::Player, this));
}

std::string Player::Upgrade(Powerup::Type type) {
    if (type == Powerup::Type::Damage) {
        attack++;
        return "+1 Attack";
    }

    if (type == Powerup::Type::Ammo) {
        max_ammo += Powerup::ammo_cap_increase;
        return "+" + std::to_string(Powerup::ammo_cap_increase) +
               " ammo storage";
    }

    if (type == Powerup::Type::Fuel) {
        max_fuel += Powerup::fuel_cap_increase;
        return "+" + std::to_string(Powerup::fuel_cap_increase / 10) +
               " fuel storage";
    }

    if (type == Powerup::Type::Health) {
        // Health powerup restores health or increases health capacity
        // if health is already max

        int heal;
        int health_upgrade;
        if (health + Powerup::health_restore > max_health) {
            heal = max_health - health;
        } else {
            heal = Powerup::health_restore;
        }
        health_upgrade = Powerup::health_restore - heal;

        health += heal;
        max_health += health_upgrade;
        std::string message = "";
        if (heal > 0) {
            message += "Restore " + std::to_string(heal) +
                       " health";
        }
        if (health_upgrade > 0) {
            message += "Max health increased by " +
                       std::to_string(health_upgrade) + " health";
        }
        return message;
    }

    if (type == Powerup::Type::Spray) {
        // Spray is how bullets are shot from the ship
        // 1 bullet, 2 bullet, etc.
        spray++;
        if (spray > max_spray) {
            spray = max_spray;
        } else {
            return "Attack powerup!";
        }
    }

    return "";
}

int Player::GetMaxHealth() { return max_health; }

void Player::SetMaxHealth(int max_health) { this->max_health = max_health; }

int Player::GetHealth() { return health; }

void Player::SetHealth(int health) { this->health = health; }

int Player::GetMaxFuel() { return max_fuel; }

void Player::SetMaxFuel(int max_fuel) { this->max_fuel = max_fuel; }

int Player::GetFuel() { return fuel; }

void Player::SetFuel(int fuel) { this->fuel = fuel; }

int Player::GetMaxAmmo() { return max_ammo; }

void Player::SetMaxAmmo(int max_ammo) { this->max_ammo = max_ammo; }

int Player::GetAmmo() { return ammo; }

void Player::SetAmmo(int ammo) { this->ammo = ammo; }

int Player::GetFuelRefresh() { return fuel_refresh; }

void Player::SetFuelRefresh(int fuel_refresh) {
    this->fuel_refresh = fuel_refresh;
}

int Player::GetAmmoRefresh() { return ammo_refresh; }

void Player::SetAmmoRefresh(int ammo_refresh) {
    this->ammo_refresh = ammo_refresh;
}

int Player::GetAttack() { return attack; }

void Player::SetAttack(int attack) { this->attack = attack; }

int Player::GetSpray() { return spray; }

void Player::SetSpray(int spray) { this->spray = spray; }