#include "EnemySpawner.hpp"
#include <chrono>
#include <random>

EnemySpawner::EnemySpawner(PoolManager& pools)
    : m_pools(&pools)
{
    m_rng.seed(std::random_device{}());
}

void EnemySpawner::addEnemyType(int cost, std::function<void(sf::Vector2f)> spawnFunc) {
    m_enemyTypes.push_back({ cost, spawnFunc });
}

void EnemySpawner::setPointGainInterval(float seconds) {
    m_pointGainInterval = seconds;
}

void EnemySpawner::update(float dt) {
    m_pointGainTimer += dt;
    if (m_pointGainTimer >= m_pointGainInterval) {
        m_pointGainTimer -= m_pointGainInterval;
        m_points += static_cast<int>(m_pointGainAmount);
        m_pointGainAmount = static_cast<int>(m_pointGainAmount * m_pointMultiplier); 
    }

    trySpawnEnemy();
}

void EnemySpawner::trySpawnEnemy() {
    if (m_enemyTypes.empty()) return;

    std::shuffle(m_enemyTypes.begin(), m_enemyTypes.end(), m_rng);

    for (auto& type : m_enemyTypes) {
        if (m_points >= type.cost) {
            std::uniform_real_distribution<float> xDist(50.f, 1230.f);
            float x = xDist(m_rng);
            type.spawnFunc({ x, -50.f });

            m_points -= type.cost;
            break; 
        }
    }
}
