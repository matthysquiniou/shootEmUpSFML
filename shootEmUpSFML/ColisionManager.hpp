#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include "Pool.hpp"
#include "SpriteComposite.hpp"

class ColisionManager {
public:
    PoolManager pools;

    void update();
};
