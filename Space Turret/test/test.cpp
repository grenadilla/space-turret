#include "catch.hpp"

#include "ofMain.h"
#include "calculations.h"

TEST_CASE("Gravity calculations work") { 
	ofVec2f player_position(0, 0);
    ofVec2f planet_position(3, 4);
    double multiplier = 250;

	ofVec2f calculated_gravity =
        calc::gravity(multiplier, player_position, planet_position);

	ofVec2f expected_gravity(6, 8);

	REQUIRE(calculated_gravity == expected_gravity);
}