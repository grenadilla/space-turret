#pragma once
#include "ofMain.h"

namespace calc {
constexpr double kDegreeRadMult = PI / 180;
constexpr int kLeftCornerDegree = 225;

ofVec2f gravity(double multiplier, ofVec2f player_position,
                ofVec2f planet_position);

int GetAngle(int x, int y);
}