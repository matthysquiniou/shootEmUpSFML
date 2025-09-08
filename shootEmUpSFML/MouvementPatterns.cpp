#include "MovementPatterns.hpp"
#include <cmath>
#include "Entity.hpp"
#include "randomGenerator.hpp"
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

    MovementPattern moveToRandom(float speed, float minX, float maxX, float minY, float maxY) {
        return [=](Entity& e, float dt, PatternState& state) {
            auto pos = e.getPosition();

            if (!state.init) {
                state.init = true;
                state.acc1 = RandomGenerator::getFloat(minX, maxX);
                state.acc2 = RandomGenerator::getFloat(minY, maxY);
            }

            float dx = state.acc1 - pos.x;
            float dy = state.acc2 - pos.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist < 1.f) {
                state.acc1 = RandomGenerator::getFloat(minX, maxX);
                state.acc2 = RandomGenerator::getFloat(minY, maxY);
            }
            else {
                float dirX = dx / dist;
                float dirY = dy / dist;

                e.move({ dirX * speed * dt, dirY * speed * dt });
            }
            };
    }

    MovementPattern bounce(float speedX, float speedY, float minX, float maxX, float minY, float maxY) {
        return [=](Entity& e, float dt, PatternState& state) {
            auto pos = e.getPosition();

            if (!state.init) {
                state.init = true;
                state.xVelocity = speedX;
                state.yVelocity = speedY;
            }

            ;

            if (pos.x > maxX) {
                state.xVelocity = -std::abs(state.xVelocity);
            } else if (pos.x < minX) {
                state.xVelocity = std::abs(state.xVelocity);
            }

            if (pos.y > maxY) {
                state.yVelocity = -std::abs(state.yVelocity);
            } else if (pos.y < minY) {
                state.yVelocity = std::abs(state.yVelocity);
            }

            e.move({ state.xVelocity * dt, state.yVelocity * dt });
            };
    }

    
}