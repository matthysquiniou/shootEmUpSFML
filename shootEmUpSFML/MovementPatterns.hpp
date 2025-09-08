#pragma once
#include <functional>

class Entity;

struct PatternState {
    int direction = 1;
    float angle = 90.f;
    float yVelocity = 0.f;
    float xVelocity = 0.f;
    float acc1 = 0.f;
    float acc2 = 0.f;
    bool init = false;
};

using MovementPattern = std::function<void(Entity&, float, PatternState&)>;

namespace MovementPatterns {

    MovementPattern linearAngleDirection(float speed);

    MovementPattern linearAngleDirectionAccelerate(float speed, float accelerate);

    MovementPattern cShape(float speedX, float speedY, float deltaSpeedX);

    MovementPattern moveToRandom(float speed, float minX, float maxX, float minY, float maxY);

    MovementPattern bounce(float speedX, float speedY, float minX, float maxX, float minY, float maxY);
} 
