#pragma once
#include "Pool.hpp"   
#include <vector>
#include <memory>
#include <random>

struct EnemyType {
    int cost;            
    std::function<void(sf::Vector2f)> spawnFunc;
};

class EnemySpawner {
public:
    EnemySpawner(PoolManager& pools);

    void update(float dt);

    void addEnemyType(int cost, std::function<void(sf::Vector2f)> spawnFunc);

    void setPointGainInterval(float seconds);

private:
    PoolManager* m_pools;

    std::vector<EnemyType> m_enemyTypes;

    int m_points = 0;
    float m_pointGainTimer = 0.f;
    float m_pointGainInterval = 10.f; 
    int m_pointGainAmount = 50;    
    float m_pointMultiplier = 1.05f; 

    std::mt19937 m_rng;

    void trySpawnEnemy();
};
