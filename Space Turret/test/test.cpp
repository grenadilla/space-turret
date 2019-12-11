#include "catch.hpp"

#include "ofMain.h"
#include "calculations.h"

TEST_CASE("Gravity calculations work") {
    SECTION("Vector <3,4> works") {
        ofVec2f player_position(0, 0);
        ofVec2f planet_position(3, 4);
        double multiplier = 250;

        ofVec2f calculated_gravity =
            calc::Gravity(multiplier, player_position, planet_position);

        ofVec2f expected_gravity(6, 8);

        REQUIRE(calculated_gravity.x == Approx(expected_gravity.x));
        REQUIRE(calculated_gravity.y == Approx(expected_gravity.y));
    }

    SECTION("Vector <7,24> works") {
        ofVec2f player_position(0, 0);
        ofVec2f planet_position(7, 24);
        double multiplier = 62500;

        ofVec2f calculated_gravity =
            calc::Gravity(multiplier, player_position, planet_position);

        ofVec2f expected_gravity(28, 96);

        REQUIRE(calculated_gravity.x == Approx(expected_gravity.x));
        REQUIRE(calculated_gravity.y == Approx(expected_gravity.y));
    }
}

TEST_CASE("Angle calculations work") {
    SECTION("Vector <2,2> works") { 
		REQUIRE(calc::GetAngle(2, 2) == 45);
	}

	SECTION("Vector <0,1> works") { 
		REQUIRE(calc::GetAngle(0, 1) == 90);
	}

	SECTION("Vector <15, 26> works") { 
		REQUIRE(calc::GetAngle(15, 26) == 60); 
	}
}