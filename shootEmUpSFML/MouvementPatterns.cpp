#include "MovementPatterns.hpp"
#include <cmath>
#include "Entity.hpp"
#include <iostream>

namespace MovementPatterns {

    constexpr float M_PI = 3.14159265f;

    MovementPattern linear(float speed) {
        return [speed](Entity& e, float dt, PatternState&) {
            e.move({ 0.f, speed * dt });
            };
    }

    MovementPattern oscillate(float speed, float amplitude) {
        return [speed, amplitude](Entity& e, float dt, PatternState& s) {
            s.time += dt;
            e.move({ amplitude * std::sin(s.time), speed * dt });
            };
    }

    MovementPattern pingPongY(float speed, float minY, float maxY) {
        return [speed, minY, maxY](Entity& e, float dt, PatternState& s) {
            e.move({ 0.f, speed * s.direction * dt });
            if (e.getPosition().y > maxY) s.direction = -1;
            if (e.getPosition().y < minY) s.direction = 1;
            };
    }

}