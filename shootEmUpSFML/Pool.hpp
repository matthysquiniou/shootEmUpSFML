#pragma once
#include "Entity.hpp"
#include <functional>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

class Pool {
public:
    using MovementPattern = std::function<void(Entity&, float)>;
    using BulletSpawner = std::function<void(Entity&, Pool&, float)>;

    Pool(
        std::size_t capacity = 256,
        Entity::Type entityType = Entity::Type::Player,
        std::shared_ptr<SpriteComposite> sprite = nullptr,
        MovementPattern pattern = nullptr,
        BulletSpawner bulletSpawner = nullptr,
        sf::Vector2f hitbox = { 0.f,0.f },
        sf::Vector2f hurtbox = { 0.f,0.f },
        sf::Vector2f boxOffSet = { 0.f,0.f },
        int health = 1,
        int damage = 1,
        float fireRate = 0.f,
        std::shared_ptr<Pool> bulletPool = nullptr,
        std::shared_ptr<Pool> destructionPool = nullptr,
        bool desactivateAfterAnimation = false
    );

    std::shared_ptr<Entity> spawn(const sf::Vector2f& pos);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    const std::vector<std::shared_ptr<Entity>>& getPool() const;

private:
    std::vector<std::shared_ptr<Entity>> pool;
    std::shared_ptr<SpriteComposite> sprite;
    MovementPattern pattern;
    BulletSpawner bulletSpawner;
    sf::Vector2f hitbox;
    sf::Vector2f hurtbox;
    sf::Vector2f boxOffSet;
    int health;
    int damage;
    float fireRate;
    std::shared_ptr<Pool> bulletPool;
    std::shared_ptr<Pool> destructionPool;
    bool desactivateAfterAnimation;
};

class PoolManager {
public:
    using BulletSpawner = std::function<void(Entity&, Pool&, float)>;

    Pool player;
    Pool playerBullet;
    Pool fighter;
    std::shared_ptr<Pool> fighterBullet;
    std::shared_ptr<Pool> fighterDestruction;

    PoolManager();

    // Helpers pour créer les sprites
    static std::shared_ptr<SpriteComposite> createPlayerSprite();
    static std::shared_ptr<SpriteComposite> createPlayerBulletSprite();
    static std::shared_ptr<SpriteComposite> createFighterSprite();
    static std::shared_ptr<SpriteComposite> createFighterBulletSprite();
    static std::shared_ptr<SpriteComposite> createFighterDestructionSprite();

    static BulletSpawner fighterBulletSpawner;
};
