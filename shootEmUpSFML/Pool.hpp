#pragma once
#include "Entity.hpp"
#include <functional>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "MovementPatterns.hpp"

class Pool {
public:
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
        bool desactivateAfterAnimation = false,
        PatternState patternState = PatternState()
    );

    std::shared_ptr<Entity> spawn(const sf::Vector2f& pos, std::shared_ptr<PatternState> ps = nullptr );
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
    PatternState patternState;
};

class PoolManager {
public:
    using BulletSpawner = std::function<void(Entity&, Pool&, float)>;

    Pool player;
    Pool playerBullet;

    Pool fighter;
    std::shared_ptr<Pool> fighterBullet;
    std::shared_ptr<Pool> fighterDestruction;

    Pool scout;
    std::shared_ptr<Pool> scoutBullet;
    std::shared_ptr<Pool> scoutDestruction;

    Pool frigate;
    std::shared_ptr<Pool> frigateBullet;
    std::shared_ptr<Pool> frigateDestruction;

    Pool torpedo;
    std::shared_ptr<Pool> torpedoBullet;
    std::shared_ptr<Pool> torpedoDestruction;

    Pool bomber;
    std::shared_ptr<Pool> bomberBullet;
    std::shared_ptr<Pool> bomberDestruction;

    Pool battleCruiser;
    std::shared_ptr<Pool> battleCruiserBullet;
    std::shared_ptr<Pool> battleCruiserDestruction;

    PoolManager();

    // Helpers pour créer les sprites
    static std::shared_ptr<SpriteComposite> createPlayerSprite();
    static std::shared_ptr<SpriteComposite> createPlayerBulletSprite();

    static std::shared_ptr<SpriteComposite> createFighterSprite();
    static std::shared_ptr<SpriteComposite> createFighterBulletSprite();
    static std::shared_ptr<SpriteComposite> createFighterDestructionSprite();

    static BulletSpawner fighterBulletSpawner;

    static std::shared_ptr<SpriteComposite> createScoutSprite();
    static std::shared_ptr<SpriteComposite> createScoutBulletSprite();
    static std::shared_ptr<SpriteComposite> createScoutDestructionSprite();

    static BulletSpawner scoutBulletSpawner;

    static std::shared_ptr<SpriteComposite> createFrigateSprite();
    static std::shared_ptr<SpriteComposite> createFrigateBulletSprite();
    static std::shared_ptr<SpriteComposite> createFrigateDestructionSprite();

    static BulletSpawner frigateBulletSpawner;

    static std::shared_ptr<SpriteComposite> createTorpedoSprite();
    static std::shared_ptr<SpriteComposite> createTorpedoBulletSprite();
    static std::shared_ptr<SpriteComposite> createTorpedoDestructionSprite();

    static BulletSpawner torpedoBulletSpawner;

    static std::shared_ptr<SpriteComposite> createBomberSprite();
    static std::shared_ptr<SpriteComposite> createBomberBulletSprite();
    static std::shared_ptr<SpriteComposite> createBomberDestructionSprite();

    static BulletSpawner bomberBulletSpawner;

    static std::shared_ptr<SpriteComposite> createBattleCruiserSprite();
    static std::shared_ptr<SpriteComposite> createBattleCruiserBulletSprite();
    static std::shared_ptr<SpriteComposite> createBattleCruiserDestructionSprite();

    static BulletSpawner battleCruiserBulletSpawner;

};
