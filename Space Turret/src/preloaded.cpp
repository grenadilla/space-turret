#include "preloaded.h"

void Preloaded::SetCollided(bool collided) { this->collided = collided; }

bool Preloaded::DidCollide() { return collided; }

bool Preloaded::IsInUse() { return in_use; }