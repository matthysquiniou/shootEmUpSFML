#pragma once
#include <random>

class RandomGenerator {
public:
    static int getInt(int min, int max);
    static float getFloat(float min, float max);

private:
    RandomGenerator() = default;
    static std::mt19937& rng();
};
