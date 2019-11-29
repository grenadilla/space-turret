#include "ofApp.h"
#include <iostream>
#include "calculations.h"
#include "identifier.h"

const double kDegreeRadMult = PI / 180;

constexpr float fps = 60.0;

constexpr int fuel_planet_radius = 60;
constexpr int ammo_planet_radius = 60;
constexpr int player_ship_radius = 20;

const std::pair<int, int> fuel_planet_coord(700, 300);
const std::pair<int, int> ammo_planet_coord(200, 300);
const std::pair<int, int> player_start_coord(400, 300);

constexpr int fuel_planet_gravity = 250;
constexpr int ammo_planet_gravity = 250;

constexpr int max_speed = 10;
constexpr int engine_force_mult = 20;
constexpr int rotate_speed = 5;

constexpr int bullet_height = 10;
constexpr int bullet_width = 4;

constexpr int total_bullets = 30;
constexpr int bullet_speed = 20;
// Maybe track time since last bullet fired
constexpr int bullet_interval = 10;
int bullet_timer = 0;

//--------------------------------------------------------------
void ofApp::setup() {
    // Background music: Tilt by Avaren https://www.avarenmusic.co/
    background_music.load("Tilt.mp3");
    background_music.play();
    background_music.setLoop(true);

    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofSetLogLevel(OF_LOG_NOTICE);

    box2d.init();
    box2d.setGravity(0, 0);
    box2d.setFPS(fps);
    box2d.registerGrabbing();
    box2d.enableEvents();

    ofAddListener(box2d.contactStartEvents, this, &ofApp::contactStart);

    fuel_planet = make_shared<Planet>(box2d.getWorld(), fuel_planet_coord.first,
                         fuel_planet_coord.second, fuel_planet_radius);

	ammo_planet = make_shared<Planet>(box2d.getWorld(), ammo_planet_coord.first,
                            ammo_planet_coord.second, ammo_planet_radius);

    player_ship = make_shared<ofxBox2dCircle>();

    player_ship->setPhysics(1.0, 0.2, 0.1);
    player_ship->setup(box2d.getWorld(), player_start_coord.first,
                      player_start_coord.second, player_ship_radius);

	//Set up bullets
    bullet_index = 0;
    for (int i = 0; i < total_bullets; i++) {
        makeBullet();
	}
    bullet_index = 0;

	Identifier *player_identifier =
        new Identifier(Identifier::ShapeType::Player, player_ship.get());

    player_ship->body->SetUserData(player_identifier);

	//Set fixed rotation so player ship doesn't roll around with friction
    player_ship->setFixedRotation(true);
}

//--------------------------------------------------------------
void ofApp::update() {
    box2d.update();

    // Remove out of bounds bullets
    removeBullets();

    // Shooting
    if (bullet_timer > 0) {
        bullet_timer--;
    }

    if (bullet_timer == 0 && keys_pressed.count(' ')) {
        auto bullet = bullets[bullet_index];
        bullet->Shoot(player_ship->getPosition().x,
                      player_ship->getPosition().y, player_ship->getRadius(),
                      player_ship->getRotation(), bullet_speed);
        bullet_index++;
        if (bullet_index >= bullets.size()) {
            bullet_index = 0;
		}
        bullet_timer = bullet_interval;
    }

    // Movement
    // std::set::count returns number of occurences, which is always zero
    // or one, so we can use it as a boolean for easier readbility
    // than using std::set::find and comparing to std::set::end
    if (keys_pressed.count(OF_KEY_LEFT) || keys_pressed.count('a')) {
        player_ship->setRotation(player_ship->getRotation() - rotate_speed);
    }

    if (keys_pressed.count(OF_KEY_RIGHT) || keys_pressed.count('d')) {
        player_ship->setRotation(player_ship->getRotation() + rotate_speed);
    }

    if (keys_pressed.count(OF_KEY_UP) || keys_pressed.count(OF_KEY_DOWN) ||
        keys_pressed.count('w') || keys_pressed.count('s')) {

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
            std::cos(player_ship->getRotation() * kDegreeRadMult) * scalar_mult,
            std::sin(player_ship->getRotation() * kDegreeRadMult) * scalar_mult);

        player_ship->addForce(force_vec, 1);

        // Enforce max velocity
        if (player_ship->getVelocity().length() > max_speed) {
            // Rescale vectors using this equation:
            // new_vector = required_length/old_magnitude * old_vector

            player_ship->setVelocity(
                player_ship->getVelocity() *
                (max_speed / player_ship->getVelocity().length()));
        }
    }

    // Calculate gravity force from the planets using inverse square law
    ofVec2f fuel_gravity_force =
        calc::gravity(fuel_planet_gravity, player_ship->getB2DPosition(),
                      fuel_planet->getB2DPosition());
    ofVec2f ammo_gravity_force =
        calc::gravity(ammo_planet_gravity, player_ship->getB2DPosition(),
                      ammo_planet->getB2DPosition());

    player_ship->addForce(fuel_gravity_force + ammo_gravity_force, 1);
}

void ofApp::contactStart(ofxBox2dContactArgs& e) { 
    Identifier *id_a = static_cast<Identifier*>(e.a->GetBody()->GetUserData());
    Identifier *id_b = static_cast<Identifier*>(e.b->GetBody()->GetUserData());

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
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofFill();

    ofSetHexColor(0x800080);
    player_ship->draw();

    for (auto bullet : bullets) {
        if (bullet->IsInUse()) {
            bullet->draw();
		}
    }

    ofSetHexColor(0x90d4e3);

    ammo_planet->draw();
    fuel_planet->draw();
}

void ofApp::removeBullets() {
	//Remove bullets if out of bounds or collided
    for (int i = 0; i < bullets.size(); i++) {
        ofVec2f position = bullets[i]->getPosition();
        if (position.x < 0 || position.x > ofGetWindowWidth() ||
            position.y < 0 || position.y > ofGetWindowHeight() ||
			bullets[i]->DidCollide()) {
            bullets[i]->Reset();
        }
    }
}

void ofApp::makeBullet() {
    auto new_bullet = std::make_shared<Bullet>(box2d.getWorld(), 
		bullet_height, bullet_width);

    bullets.push_back(new_bullet);
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
