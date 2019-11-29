#pragma once

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