#include "ofApp.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofSetLogLevel(OF_LOG_NOTICE);

    box2d.init();
    box2d.setGravity(0, 0);
    box2d.setFPS(60.0);
    box2d.registerGrabbing();

	fuel_planet = ofxBox2dCircle();
    ammo_planet = ofxBox2dCircle();
    player_ship = ofxBox2dCircle();

	//fuel_planet.setPhysics(0.0, 0.0, 0.1);
    fuel_planet.setup(box2d.getWorld(), 500, 200, 70);
    fuel_planet.body->SetType(b2_staticBody);

	ammo_planet.setup(box2d.getWorld(), 300, 600, 70);
    ammo_planet.body->SetType(b2_staticBody);

	player_ship.setPhysics(1.0, 0.2, 0.1);
    player_ship.setup(box2d.getWorld(), 300, 300, 20);
}

//--------------------------------------------------------------
void ofApp::update() {
    box2d.update();

    if (keys_pressed.left) {
        player_ship.setRotation(player_ship.getRotation() - 3);
    }
    if (keys_pressed.right) {
        player_ship.setRotation(player_ship.getRotation() + 3);
    }
    if (keys_pressed.up || keys_pressed.down) {
        int scalar_mult;
        if (keys_pressed.up) {
            scalar_mult = 20;
        } else {
            scalar_mult = -20;
        }

		//Note that rotation is measured by ofxBox2D in degrees instead of radians
        ofVec2f force_vec;
        force_vec.set(std::cos(player_ship.getRotation() * kDegreeRadMult)  * scalar_mult,
                      std::sin(player_ship.getRotation() * kDegreeRadMult) * scalar_mult);

		
        player_ship.addForce(force_vec, 1);

		//Enforce max velocity
        if (player_ship.getVelocity().length() > 10) {
			//Rescale vectors using this equation:
			// new_vector = required_length/old_magnitude * old_vector

            player_ship.setVelocity(player_ship.getVelocity() *
                                    (10 / player_ship.getVelocity().length()));
		}
    }
}

//--------------------------------------------------------------
void ofApp::draw() { 
	ofFill();

	ofSetHexColor(0x800080);
    player_ship.draw();

    ofSetHexColor(0x90d4e3);

	//Can draw circles with .draw() for debug or next one for release
	fuel_planet.draw(); 
	ammo_planet.draw();
	//ofDrawCircle(fuel_planet.getPosition(), fuel_planet.getRadius());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == OF_KEY_LEFT) {
        keys_pressed.left = true;
    }
    if (key == OF_KEY_RIGHT) {
        keys_pressed.right = true;
    }
    if (key == OF_KEY_UP) {
        keys_pressed.up = true;
    }
    if (key == OF_KEY_DOWN) {
        keys_pressed.down = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == OF_KEY_LEFT) {
        keys_pressed.left = false;
    }
    if (key == OF_KEY_RIGHT) {
        keys_pressed.right = false;
    }
    if (key == OF_KEY_UP) {
        keys_pressed.up = false;
    }
    if (key == OF_KEY_DOWN) {
        keys_pressed.down = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
