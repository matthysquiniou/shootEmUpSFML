#pragma once
#include <functional>

class Entity;

struct PatternState {
    float time = 0.f;
    int direction = 1;
    float angle = 0.f;
};

using MovementPattern = std::function<void(Entity&, float, PatternState&)>;

namespace MovementPatterns {

    MovementPattern linear(float speed);
    MovementPattern oscillate(float speed, float amplitude);
    MovementPattern pingPongY(float speed, float minY, float maxY);

} 
