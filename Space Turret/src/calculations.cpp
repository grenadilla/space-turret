#include "calculations.h"

namespace calc {
	ofVec2f gravity(double multiplier, ofVec2f player_position,
		ofVec2f planet_position) {
		ofVec2f gravity_force = planet_position - player_position;
            gravity_force =
                gravity_force.getNormalized() / gravity_force.lengthSquared();
            return gravity_force * multiplier;
        }
}