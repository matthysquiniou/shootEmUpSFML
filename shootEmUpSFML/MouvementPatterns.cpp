#include "MovementPatterns.hpp"
#include <cmath>
#include "Entity.hpp"
#include <iostream>

namespace MovementPatterns {

    constexpr float M_PI = 3.14159265f;

    MovementPattern linearAngleDirection(float speed) {
        return [speed](Entity& e, float dt, PatternState& state) {
            if (!state.init) {
                state.init = true;
                float rad = state.angle * 3.14159265f / 180.f;
                state.xVelocity = std::cos(rad) * speed;
                state.yVelocity = std::sin(rad) * speed;
            }

            e.move({ state.xVelocity * dt, state.yVelocity * dt });
            };
    }

    MovementPattern linearAngleDirectionAccelerate(float speed, float accelerate) {
        return [speed, accelerate](Entity& e, float dt, PatternState& state) {

            if (!state.init) {
                state.init = true;
                float rad = state.angle * 3.14159265f / 180.f;
                state.acc1 = std::cos(rad);
                state.acc2 = std::sin(rad);

                state.xVelocity = state.acc1 * speed;
                state.yVelocity = state.acc2 * speed;
            }

            e.move({ state.xVelocity * dt, state.yVelocity * dt });
            state.xVelocity += state.acc1 * accelerate;
            state.yVelocity += state.acc2 * accelerate;
            };
    }


    MovementPattern cShape(float speedX, float speedY, float deltaSpeedX) {
        return [speedX, speedY, deltaSpeedX](Entity& e, float dt, PatternState& state) {

            if (!state.init) {
                state.init = true;
                state.xVelocity = speedX;
            }
            e.move({ state.xVelocity * state.direction * dt, speedY * dt });

            state.xVelocity -= deltaSpeedX * dt;

            };

    }

    
}