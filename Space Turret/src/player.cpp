#include "player.h";

#include "identifier.h"
#include "ofMain.h"

Player::Player() = default;

Player::Player(b2World* b2dWorld, int x, int y, int radius, int health,
               int fuel, int ammo, int fuel_refresh, int ammo_refresh,
               float density, float bounce, float friction) {
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

    // Set collision filters
    b2Filter filter;
    filter.categoryBits = Identifier::player_category;
    setFilterData(filter);

    setData(new Identifier(Identifier::ShapeType::Player, this));
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

void Player::SetFuelRefresh(int fuel_refresh) { this->fuel_refresh = fuel_refresh; }

int Player::GetAmmoRefresh() { return ammo_refresh; }

void Player::SetAmmoRefresh(int ammo_refresh) { this->ammo_refresh = ammo_refresh; }