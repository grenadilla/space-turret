#include "calculations.h"

#include <cmath>

const double kDegreeRadMult = PI / 180;
constexpr int kDegreeInCircle = 360;

namespace calc {
	ofVec2f gravity(double multiplier, ofVec2f player_position,
		ofVec2f planet_position) {
		ofVec2f gravity_force = planet_position - player_position;
            gravity_force =
                gravity_force.getNormalized() / gravity_force.lengthSquared();
            return gravity_force * multiplier;
    }

	int GetAngle(int x, int y) {
        // Get angle in radians, convert to degrees, and set to between 0 and
        // 360
        int angle = (int) (std::atan((double) y / x) / kDegreeRadMult);

        // Range of arctan is from -90 to 90 degrees, so we have to check for
        // 180 to 270 degrees
        if (x < 0) {
            angle += 180;
        }

        angle = (angle + kDegreeInCircle) % kDegreeInCircle;
        return angle;
	}
}