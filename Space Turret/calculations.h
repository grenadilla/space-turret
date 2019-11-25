#pragma once
#include "ofMain.h"

namespace calc {
ofVec2f gravity(double multiplier, ofVec2f player_position,
                ofVec2f planet_position);
}