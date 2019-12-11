#pragma once

// Preloaded is an abstract class (interface) for objects that are recycled
// as they're used. Put them in a vector, keep track of the index, and keep using them.
// We preload so as not to cause memory leaks that would result from dynamically generating
// the objects, as they hold Identifiers

class Preloaded {
   public:
    virtual void Reset() = 0;
    bool DidCollide();
    void SetCollided(bool collided);
    bool IsInUse();

   protected:
    bool collided;
    bool in_use;
};