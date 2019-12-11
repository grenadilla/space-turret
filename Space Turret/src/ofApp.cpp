#include "ofApp.h"

#include <iostream>
#include <map>
#include <string>

#include "calculations.h"
#include "identifier.h"

constexpr float fps = 60.0;

constexpr int fuel_planet_radius = 60;
constexpr int ammo_planet_radius = 60;
constexpr int player_ship_radius = 20;

const std::pair<int, int> fuel_planet_coord(700, 400);
const std::pair<int, int> ammo_planet_coord(300, 300);
const std::pair<int, int> player_start_coord(500, 300);

constexpr int player_start_health = 10;
constexpr int player_start_fuel = 1000;
constexpr int player_start_ammo = 10;
constexpr int player_fuel_refresh = 3;
constexpr int player_ammo_refresh = 1;

constexpr float player_density = 1;
constexpr float player_bounce = 0.2;
constexpr float player_friction = 0.1;

constexpr int fuel_planet_gravity = 170;
constexpr int ammo_planet_gravity = 170;
constexpr int planet_color = 0x90d4e3;

constexpr int max_speed = 10;
constexpr int engine_force_mult = 20;
constexpr int rotate_speed = 4;

constexpr int bullet_height = 10;
constexpr int bullet_width = 4;

// Number of bullets to preload
constexpr int total_bullets = 30;
constexpr int bullet_speed = 20;

// Maybe track time since last bullet fired
constexpr int bullet_interval = 10;
int bullet_timer = 0;

const std::vector<int> enemy_colors = {0x4fd1cf, 0x79d14f, 0xd46a0d, 0xbd2020};
constexpr int enemy_size = 40;
constexpr int total_enemies = 15;
constexpr int enemy_speed = 1;
constexpr double start_spawn_rate = 0.003;
constexpr double spawn_rate_increase = 0.0005;
constexpr int difficulty_increase_duration = 4000;
constexpr double spawn_boundary_prop = 0.05;

constexpr int total_powerups = 5;
constexpr int powerup_radius = 20;
constexpr double powerup_drop_rate = 0.2;
constexpr double spray_drop_rate = 0.2;
const std::map<Powerup::Type, int> powerup_colors = {
    std::make_pair(Powerup::Type::Ammo, 0xe61045),
    std::make_pair(Powerup::Type::Damage, 0x1f31b8),
    std::make_pair(Powerup::Type::Fuel, 0x9e6e33),
    std::make_pair(Powerup::Type::Health, 0xed3494),
    std::make_pair(Powerup::Type::Spray, 0x34edc2)};

constexpr int font_size = 12;
constexpr int game_over_font_size = 40;
constexpr int message_display_time = 180;
const std::string game_over_message = "Game Over!";

//--------------------------------------------------------------
void ofApp::setup() {
    // Background music: Tilt by Avaren https://www.avarenmusic.co/
    background_music.load("sounds/Tilt.mp3");
    background_music.play();
    background_music.setLoop(true);
    damage_sound.setVolume(0.7);

    laser_sound.load("sounds/laser.wav");
    laser_sound.setMultiPlay(true);
    laser_sound.setVolume(0.2);

    powerup_sound.load("sounds/powerup.wav");
    powerup_sound.setMultiPlay(true);
    damage_sound.setVolume(0.7);

    damage_sound.load("sounds/damage.wav");
    damage_sound.setMultiPlay(true);

    explosion_sound.load("sounds/explosion.wav");
    explosion_sound.setMultiPlay(true);

    // Load font
    ofTrueTypeFont::setGlobalDpi(72);
    font.load("ibm_bios.ttf", font_size);

    // Load images
    blast.load("images/blast.png");
    ion.load("images/ion-cannon-blast.png");
    heart_plus.load("images/heart-plus.png");
    oil_drum.load("images/oil-drum.png");
    heavy_bullets.load("images/heavy-bullets.png");
    std::map<Powerup::Type, ofImage> images;
    images[Powerup::Type::Damage] = ion;
    images[Powerup::Type::Spray] = blast;
    images[Powerup::Type::Fuel] = oil_drum;
    images[Powerup::Type::Health] = heart_plus;
    images[Powerup::Type::Ammo] = heavy_bullets;
    Powerup::LoadImages(images);

    fuel_icon.load("images/fuel-tank.png");
    ammo_icon.load("images/reload-gun-barrel.png");

    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofSetLogLevel(OF_LOG_NOTICE);

    box2d.init();
    box2d.setGravity(0, 0);
    box2d.setFPS(fps);
    box2d.registerGrabbing();
    box2d.enableEvents();

    ofAddListener(box2d.contactStartEvents, this, &ofApp::contactStart);
    ofAddListener(box2d.contactEndEvents, this, &ofApp::contactEnd);

    fuel_planet = make_shared<Planet>(
        box2d.getWorld(), fuel_planet_coord.first, fuel_planet_coord.second,
        fuel_planet_radius, planet_color, &fuel_icon);

    ammo_planet = make_shared<Planet>(
        box2d.getWorld(), ammo_planet_coord.first, ammo_planet_coord.second,
        ammo_planet_radius, planet_color, &ammo_icon);

    player_ship = make_shared<Player>(
        box2d.getWorld(), player_start_coord.first, player_start_coord.second,
        player_ship_radius, player_start_health, player_start_fuel,
        player_start_ammo, player_fuel_refresh, player_ammo_refresh, 1,
        player_density, player_bounce, player_friction);

    Preload();

    difficulty_increase_timer = difficulty_increase_duration;
    spawn_rates.push_back(start_spawn_rate);

    game_over = false;
}

//--------------------------------------------------------------
void ofApp::update() {
    if (!game_over) {
        box2d.update();

        // Remove out of bounds or not in use preloaded objects
        removePreloadedObjects();

        // Spawn powerups
        if (powerup_to_drop != nullptr) {
            powerup_to_drop->Drop();
            powerup_to_drop = nullptr;
        }

        if (powerup_message_timer > 0) {
            powerup_message_timer--;
        }

        UpdateSpawnRate();

        // Randomly spawn an enemy sometimes
        SpawnEnemy();

        // Shooting
        ShootBullet();

        // Movement based on player input
        MovePlayer();

        // Wrap player around screen if their coordinates are offscreen
        WrapAroundPlayer();

        // Enforce max velocity
        if (player_ship->getVelocity().length() > max_speed) {
            // Rescale vectors using this equation:
            // new_vector = required_length/old_magnitude * old_vector

            player_ship->setVelocity(
                player_ship->getVelocity() *
                (max_speed / player_ship->getVelocity().length()));
        }

        AddGravity();

        // Check which planet the player is "touching"
        SetPlanetContact();

        // Restock fuel and ammo if player is touching correct planet
        RestockPlayer();

        if (player_ship->GetHealth() == 0) {
            game_over = true;
        }
    }
}

void ofApp::contactStart(ofxBox2dContactArgs &e) {
    Identifier *id_a = static_cast<Identifier *>(e.a->GetBody()->GetUserData());
    Identifier *id_b = static_cast<Identifier *>(e.b->GetBody()->GetUserData());

    if (id_a->GetType() == Identifier::ShapeType::Bullet &&
        id_b->GetType() == Identifier::ShapeType::Planet) {
        shared_ptr<Bullet> bullet =
            std::static_pointer_cast<Bullet>(id_a->GetShape());
        bullet->SetCollided(true);
    }

    if (id_b->GetType() == Identifier::ShapeType::Bullet &&
        id_a->GetType() == Identifier::ShapeType::Planet) {
        shared_ptr<Bullet> bullet =
            std::static_pointer_cast<Bullet>(id_b->GetShape());
        bullet->SetCollided(true);
    }

    if (id_a->GetType() == Identifier::ShapeType::Enemy &&
        id_b->GetType() == Identifier::ShapeType::Planet) {
        shared_ptr<Enemy> enemy =
            std::static_pointer_cast<Enemy>(id_a->GetShape());
        enemy->SetCollided(true);
        player_ship->SetHealth(player_ship->GetHealth() - 1);
        damage_sound.play();
    }

    if (id_b->GetType() == Identifier::ShapeType::Enemy &&
        id_a->GetType() == Identifier::ShapeType::Planet) {
        shared_ptr<Bullet> enemy =
            std::static_pointer_cast<Bullet>(id_b->GetShape());
        enemy->SetCollided(true);
        player_ship->SetHealth(player_ship->GetHealth() - 1);
        damage_sound.play();
    }

    if (id_a->GetType() == Identifier::ShapeType::Bullet &&
        id_b->GetType() == Identifier::ShapeType::Enemy) {
        shared_ptr<Bullet> bullet =
            std::static_pointer_cast<Bullet>(id_a->GetShape());
        shared_ptr<Enemy> enemy =
            std::static_pointer_cast<Enemy>(id_b->GetShape());
        bullet->SetCollided(true);
        if (enemy->Damage(player_ship->GetAttack())) {
            SpawnPowerup(enemy->getPosition().x, enemy->getPosition().y);
            explosion_sound.play();
        }
    }

    if (id_b->GetType() == Identifier::ShapeType::Bullet &&
        id_a->GetType() == Identifier::ShapeType::Enemy) {
        shared_ptr<Bullet> bullet =
            std::static_pointer_cast<Bullet>(id_b->GetShape());
        shared_ptr<Enemy> enemy =
            std::static_pointer_cast<Enemy>(id_a->GetShape());
        bullet->SetCollided(true);
        if (enemy->Damage(player_ship->GetAttack())) {
            SpawnPowerup(enemy->getPosition().x, enemy->getPosition().y);
            explosion_sound.play();
        }
    }

    if (id_a->GetType() == Identifier::ShapeType::Player &&
        id_b->GetType() == Identifier::ShapeType::Enemy) {
        shared_ptr<Enemy> enemy =
            std::static_pointer_cast<Enemy>(id_b->GetShape());
        enemy->SetCollided(true);
        player_ship->SetHealth(player_ship->GetHealth() - 1);
        damage_sound.play();
    }

    if (id_b->GetType() == Identifier::ShapeType::Player &&
        id_a->GetType() == Identifier::ShapeType::Enemy) {
        shared_ptr<Enemy> enemy =
            std::static_pointer_cast<Enemy>(id_a->GetShape());
        enemy->SetCollided(true);
        player_ship->SetHealth(player_ship->GetHealth() - 1);
        damage_sound.play();
    }

    if (id_a->GetType() == Identifier::ShapeType::Player &&
        id_b->GetType() == Identifier::ShapeType::Powerup) {
        shared_ptr<Powerup> powerup =
            std::static_pointer_cast<Powerup>(id_b->GetShape());
        powerup->SetCollided(true);
        powerup_message = player_ship->Upgrade(powerup->GetType());
        powerup_message_timer = message_display_time;
        powerup_sound.play();
    }

    if (id_b->GetType() == Identifier::ShapeType::Player &&
        id_a->GetType() == Identifier::ShapeType::Powerup) {
        shared_ptr<Powerup> powerup =
            std::static_pointer_cast<Powerup>(id_a->GetShape());
        powerup->SetCollided(true);
        powerup_message = player_ship->Upgrade(powerup->GetType());
        powerup_message_timer = message_display_time;
        powerup_sound.play();
    }
}

void ofApp::contactEnd(ofxBox2dContactArgs &e) {
    Identifier *id_a = static_cast<Identifier *>(e.a->GetBody()->GetUserData());
    Identifier *id_b = static_cast<Identifier *>(e.b->GetBody()->GetUserData());

    if (id_a->GetType() == Identifier::ShapeType::Bullet &&
        id_b->GetType() == Identifier::ShapeType::Enemy) {
        shared_ptr<Enemy> enemy =
            std::static_pointer_cast<Enemy>(id_b->GetShape());
        enemy->Retarget();
    }

    if (id_b->GetType() == Identifier::ShapeType::Bullet &&
        id_a->GetType() == Identifier::ShapeType::Enemy) {
        shared_ptr<Enemy> enemy =
            std::static_pointer_cast<Enemy>(id_a->GetShape());
        enemy->Retarget();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetHexColor(0x4AF626);

    if (!game_over) {
        std::string message =
            "Health: " + std::to_string(player_ship->GetHealth()) + "/" +
            std::to_string(player_ship->GetMaxHealth());
        font.drawString(message, 20, 20);

        message = "Ammo: " + std::to_string(player_ship->GetAmmo()) + "/" +
                  std::to_string(player_ship->GetMaxAmmo());
        font.drawString(message, 20, 40);

        message = "Fuel: " + std::to_string(player_ship->GetFuel() / 10) + "/" +
                  std::to_string(player_ship->GetMaxFuel() / 10);
        font.drawString(message, 20, 60);

        message = "Attack: " + std::to_string(player_ship->GetAttack());
        font.drawString(message, 20, 80);

        if (powerup_message_timer > 0) {
            font.drawString(powerup_message, 300, 40);
        }

        ofFill();

        ofSetHexColor(0x800080);
        player_ship->draw();

        for (auto bullet : bullets) {
            if (bullet->IsInUse()) {
                bullet->draw();
            }
        }

        for (auto enemy : enemies) {
            if (enemy->IsInUse()) {
                enemy->draw();
            }
        }

        for (auto powerup : powerups) {
            if (powerup->IsInUse()) {
                powerup->draw();
            }
        }

        ammo_planet->draw();
        fuel_planet->draw();
    } else {
        font.load("ibm_bios.ttf", game_over_font_size);
        font.drawString(game_over_message, (ofGetWidth() - game_over_font_size * game_over_message.size()) / 2,
                        (ofGetHeight() - game_over_font_size) / 2);
    }
}

void ofApp::Preload() {
    // Set up bullets
    bullet_index = 0;
    Enemy::SetColors(enemy_colors);
    for (int i = 0; i < total_bullets; i++) {
        auto new_bullet = std::make_shared<Bullet>(box2d.getWorld(),
                                                   bullet_height, bullet_width);
        bullets.push_back(new_bullet);
    }

    // Set up enemies
    enemy_index = 0;
    for (int i = 0; i < total_enemies; i++) {
        auto new_enemy = std::make_shared<Enemy>(box2d.getWorld(), enemy_size);
        enemies.push_back(new_enemy);
    }

    // Set up powerups
    powerup_index = 0;
    for (int i = 0; i < total_powerups; i++) {
        auto new_powerup =
            std::make_shared<Powerup>(box2d.getWorld(), powerup_radius);
        powerups.push_back(new_powerup);
    }
}

void ofApp::removePreloadedObjects() {
    // Remove bullets if out of bounds or collided
    for (auto bullet : bullets) {
        ofVec2f position = bullet->getPosition();
        if (position.x < 0 || position.x > ofGetWindowWidth() ||
            position.y < 0 || position.y > ofGetWindowHeight() ||
            bullet->DidCollide()) {
            bullet->Reset();
        }
    }

    for (auto enemy : enemies) {
        ofVec2f position = enemy->getPosition();
        if (position.x < 0 || position.x > ofGetWindowWidth() ||
            position.y < 0 || position.y > ofGetWindowHeight() ||
            enemy->DidCollide()) {
            enemy->Reset();
        }
    }

    for (auto powerup : powerups) {
        if (powerup->DidCollide()) {
            powerup->Reset();
        }
    }
}

void ofApp::SpawnEnemy() {
    // Spawn enemies
    for (int i = 0; i < spawn_rates.size(); i++) {
        // i is health
        if (ofRandom(0, 1) <= spawn_rates[i]) {
            int x;
            int y;

            // Select if on horizontal or vertical
            if (ofRandom(0, 2) >= 1) {
                // Horizontal
                // Left or right
                if (ofRandom(0, 2) >= 1) {
                    x = spawn_boundary_prop * ofGetWidth();
                } else {
                    x = (1 - spawn_boundary_prop) * ofGetWidth();
                }
                y = ofRandom(0, 1) * ofGetHeight();
            } else {
                // Vertical
                // Top or bottom
                if (ofRandom(0, 2) >= 1) {
                    y = spawn_boundary_prop * ofGetHeight();
                } else {
                    y = (1 - spawn_boundary_prop) * ofGetHeight();
                }
                x = ofRandom(0, 1) * ofGetWidth();
            }

            // Decide which planet to target
            if (ofRandom(0, 1) >= 0.5) {
                enemies[enemy_index]->Attack(x, y, fuel_planet->getPosition().x,
                                             fuel_planet->getPosition().y,
                                             enemy_speed, i + 1);
            } else {
                enemies[enemy_index]->Attack(x, y, ammo_planet->getPosition().x,
                                             ammo_planet->getPosition().y,
                                             enemy_speed, i + 1);
            }

            enemy_index++;
            if (enemy_index >= enemies.size()) {
                enemy_index = 0;
            }
        }
    }
}

void ofApp::SpawnPowerup(int x, int y) {
    if (ofRandom(0, 1) > powerup_drop_rate) {
        return;
    }

    double randNum = ofRandom(0, 1);

    Powerup::Type type;
    if (player_ship->GetAttack() < spawn_rates.size()) {
        // Only increase player's attack if it is less than max enemy health
        type = Powerup::Type::Damage;
    }

    else if (ofRandom(0, 1) < spray_drop_rate) {
        type = Powerup::Type::Spray;
    }

    else if (randNum < 0.3) {
        type = Powerup::Type::Fuel;
    }

    else if (randNum >= 0.3 && randNum < 0.7) {
        type = Powerup::Type::Health;
    }

    else {
        type = Powerup::Type::Ammo;
    }

    // Have to use Prepare() instead of Drop() because objects cannot be moved
    // outside of setup or update, and this method is called inside an event
    // listener
    powerups[powerup_index]->Prepare(x, y, type, powerup_colors.at(type));
    powerup_to_drop = powerups[powerup_index];
    powerup_index++;
    if (powerup_index >= powerups.size()) {
        powerup_index = 0;
    }
}

void ofApp::UpdateSpawnRate() {
    difficulty_increase_timer--;

    // Increase each spawn rate by stealing half of previous spawn rate
    if (difficulty_increase_timer == 0) {
        spawn_rates[0] += spawn_rate_increase;
        double counter = 0;
        for (int i = spawn_rates.size() - 1; i >= 0; i--) {
            counter = spawn_rates[i] / 2;
            spawn_rates[i] /= 2;
            if (i > 0) {
                spawn_rates[i - 1] += counter;
            }
        }
        spawn_rates.push_back(counter);
        difficulty_increase_timer = difficulty_increase_duration;
    }
}

void ofApp::MovePlayer() {
    // std::set::count returns number of occurences, which is always zero
    // or one, so we can use it as a boolean for easier readbility
    // than using std::set::find and comparing to std::set::end
    if (keys_pressed.count(OF_KEY_LEFT) || keys_pressed.count('a')) {
        player_ship->setRotation(player_ship->getRotation() - rotate_speed);
    }

    if (keys_pressed.count(OF_KEY_RIGHT) || keys_pressed.count('d')) {
        player_ship->setRotation(player_ship->getRotation() + rotate_speed);
    }

    if ((keys_pressed.count(OF_KEY_UP) || keys_pressed.count(OF_KEY_DOWN) ||
         keys_pressed.count('w') || keys_pressed.count('s')) &&
        player_ship->GetFuel() > 0) {
        int scalar_mult;
        if (keys_pressed.count(OF_KEY_UP) || keys_pressed.count('w')) {
            scalar_mult = engine_force_mult;
        } else if (keys_pressed.count(OF_KEY_DOWN) || keys_pressed.count('s')) {
            scalar_mult = -1 * engine_force_mult;
        }

        // Note that rotation is measured by ofxBox2D in degrees instead of
        // radians
        ofVec2f force_vec;
        force_vec.set(
            std::cos(player_ship->getRotation() * calc::kDegreeRadMult) *
                scalar_mult,
            std::sin(player_ship->getRotation() * calc::kDegreeRadMult) *
                scalar_mult);

        player_ship->addForce(force_vec, 1);
        player_ship->SetFuel(player_ship->GetFuel() - 1);
    }
}

void ofApp::ShootBullet() {
    if (bullet_timer > 0) {
        bullet_timer--;
    }

    if (bullet_timer == 0 && player_ship->GetAmmo() > 0 &&
        keys_pressed.count(' ')) {
        laser_sound.play();

        auto bullet1 = bullets[bullet_index];
        auto bullet2 = bullets[(bullet_index + 1) % bullets.size()];
        auto bullet3 = bullets[(bullet_index + 2) % bullets.size()];

        // Shooting patterns:
        // 1 - Single bullet
        // 2 - Two bullets side by side, symetric with the ship
        // 3 - Three bullets, with middle one centered with ship
        if (player_ship->GetSpray() == 1 || player_ship->GetSpray() == 3) {
            bullet1->Shoot(player_ship->getPosition().x,
                           player_ship->getPosition().y,
                           player_ship->getRadius(), player_ship->getRotation(),
                           bullet_speed);
            bullet_index++;
        }

        if (player_ship->GetSpray() == 2) {
            bullet1->Shoot(player_ship->getPosition().x,
                           player_ship->getPosition().y,
                           player_ship->getRadius(), player_ship->getRotation(),
                           bullet_speed, -20);
            bullet2->Shoot(player_ship->getPosition().x,
                           player_ship->getPosition().y,
                           player_ship->getRadius(), player_ship->getRotation(),
                           bullet_speed, 20);
            bullet_index += 2;
        }

        if (player_ship->GetSpray() == 3) {
            bullet2->Shoot(player_ship->getPosition().x,
                           player_ship->getPosition().y,
                           player_ship->getRadius(), player_ship->getRotation(),
                           bullet_speed, -30);
            bullet3->Shoot(player_ship->getPosition().x,
                           player_ship->getPosition().y,
                           player_ship->getRadius(), player_ship->getRotation(),
                           bullet_speed, 30);
            bullet_index += 2;
        }

        bullet_index %= bullets.size();
        bullet_timer = bullet_interval;
        player_ship->SetAmmo(player_ship->GetAmmo() - 1);
    }
}

void ofApp::AddGravity() {
    // Calculate gravity force from the planets using inverse square law
    ofVec2f fuel_gravity_force =
        calc::gravity(fuel_planet_gravity, player_ship->getB2DPosition(),
                      fuel_planet->getB2DPosition());
    ofVec2f ammo_gravity_force =
        calc::gravity(ammo_planet_gravity, player_ship->getB2DPosition(),
                      ammo_planet->getB2DPosition());

    player_ship->addForce(fuel_gravity_force + ammo_gravity_force, 1);
}

void ofApp::WrapAroundPlayer() {
    // Wrapping player around the screen
    if (player_ship->getPosition().x < 0) {
        player_ship->setPosition(ofGetWidth(), player_ship->getPosition().y);
    }
    if (player_ship->getPosition().y < 0) {
        player_ship->setPosition(player_ship->getPosition().x, ofGetHeight());
    }
    if (player_ship->getPosition().x > ofGetWidth()) {
        player_ship->setPosition(0, player_ship->getPosition().y);
    }
    if (player_ship->getPosition().y > ofGetHeight()) {
        player_ship->setPosition(player_ship->getPosition().x, 0);
    }
}

void ofApp::SetPlanetContact() {
    // The +1 after the radius is for a small margin of error
    if (player_ship->getPosition().distance(fuel_planet->getPosition()) <=
        player_ship->getRadius() + fuel_planet->getRadius() + 1) {
        fuel_planet->SetTouchingPlayer(true);
    } else {
        fuel_planet->SetTouchingPlayer(false);
    }

    if (player_ship->getPosition().distance(ammo_planet->getPosition()) <=
        player_ship->getRadius() + ammo_planet->getRadius() + 1) {
        ammo_planet->SetTouchingPlayer(true);
    } else {
        ammo_planet->SetTouchingPlayer(false);
    }
}

void ofApp::RestockPlayer() {
    if (fuel_planet->IsTouchingPlayer()) {
        int fuel = player_ship->GetFuel() + player_ship->GetFuelRefresh();
        if (fuel > player_ship->GetMaxFuel()) {
            fuel = player_ship->GetMaxFuel();
        }
        player_ship->SetFuel(fuel);
    }

    if (ammo_planet->IsTouchingPlayer()) {
        int ammo = player_ship->GetAmmo() + player_ship->GetAmmoRefresh();
        if (ammo > player_ship->GetMaxAmmo()) {
            ammo = player_ship->GetMaxAmmo();
        }
        player_ship->SetAmmo(ammo);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) { keys_pressed.insert(key); }

//--------------------------------------------------------------
void ofApp::keyReleased(int key) { keys_pressed.erase(key); }

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
