#include "randomGenerator.hpp"
#include <random>

int RandomGenerator::getInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng());
}

float RandomGenerator::getFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng());
}

std::mt19937& RandomGenerator::rng() {
    static std::mt19937 engine{ std::random_device{}() };
    return engine;
}
