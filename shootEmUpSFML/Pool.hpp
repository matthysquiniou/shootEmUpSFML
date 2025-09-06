#pragma once
#include "Entity.hpp"

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
    ) : sprite(sprite),
        pattern(pattern),
        bulletSpawner(bulletSpawner),
        hitbox(hitbox),
        hurtbox(hurtbox),
        boxOffSet(boxOffSet),
        health(health),
        damage(damage),
        fireRate(fireRate),
        bulletPool(bulletPool),
        destructionPool(destructionPool),
        desactivateAfterAnimation(desactivateAfterAnimation)
    {
        pool.reserve(capacity);
        for (std::size_t i = 0; i < capacity; i++) {
            pool.push_back(std::make_shared<Entity>(entityType));
        }
    }

    std::shared_ptr<Entity> spawn(const sf::Vector2f& pos)
    {
        for (auto& obj : pool) {
            if (!obj->isActive()) {
                auto comp = *sprite;
                obj->setComposite(comp);
                obj->setHurtbox(hurtbox,boxOffSet);
                obj->setHitbox(hitbox,boxOffSet);
                obj->setMovementPattern(pattern);
                obj->setBulletSpawner(bulletSpawner);
                obj->setHealth(health);
                obj->setDamage(damage);
                obj->setFireRate(fireRate);
                obj->setBulletPool(bulletPool);
                obj->setDestructionPool(destructionPool);
                obj->setDesactivateAfterAnimation(desactivateAfterAnimation);
                obj->setPosition(pos);
                obj->activate();
                return obj;
            }
        }
        return nullptr; 
    }

    void update(float dt) {
        for (auto& obj : pool) {
            if (obj->isActive()) {
                obj->update(dt);

                if (obj->getPosition().y < -50.f || obj->getPosition().y > 650.f ||
                    obj->getPosition().x < -50.f || obj->getPosition().x > 850.f
                    ) {
                    obj->deactivate();
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        for (auto& obj : pool) {
            if (obj->isActive()) {
                window.draw(*obj);
            }
        }
    }

    const std::vector<std::shared_ptr<Entity>>& getPool() const { return pool; }

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

    PoolManager() {
        player = Pool(
            1,
            Entity::Type::Player,
            createPlayerSprite(),
            nullptr,
            nullptr,
            sf::Vector2f(32.f, 32.f),
            sf::Vector2f(0.f, 0.f),
            sf::Vector2f(8.f, 8.f),
            10,
            0,
            0.f,
            nullptr,
            nullptr,
            false
        );

        playerBullet = Pool(
            128,
            Entity::Type::Player,
            createPlayerBulletSprite(),
            [](Entity& e, float dt) { e.move({ 0.f, -400.f * dt }); },
            nullptr,
            sf::Vector2f(0.f, 0.f),
            sf::Vector2f(16.f, 16.f),
            sf::Vector2f(8.f, 8.f),
            1,
            1,
            0.f,
            nullptr,
            nullptr,
            false
        );

        fighterBullet = std::make_shared<Pool>(
            128,
            Entity::Type::Enemy,
            createFighterBulletSprite(),
            [](Entity& e, float dt) { e.move({ 0.f, 250.f * dt }); },
            nullptr,
            sf::Vector2f(0.f, 0.f),
            sf::Vector2f(9.f, 12.f),
            sf::Vector2f(0.f, 0.f),
            1,
            1,
            0.f,
            nullptr,
            nullptr,
            false
        );


        fighterDestruction = std::make_shared<Pool>(
            32,
            Entity::Type::Enemy,
            createFighterDestructionSprite(),
            nullptr,
            nullptr,
            sf::Vector2f(0.f, 0.f),
            sf::Vector2f(0.f, 0.f),
            sf::Vector2f(0.f, 0.f),
            1,
            0,
            0.f,
            nullptr,
            nullptr,
            true
        );

        fighter = Pool(
            32,
            Entity::Type::Enemy,
            createFighterSprite(),
            [](Entity& e, float dt) { e.move({ 0.f, 50.f * dt }); },
            fighterBulletSpawner,
            sf::Vector2f(32.f, 32.f),
            sf::Vector2f(0.f, 0.f),
            sf::Vector2f(16.f, 16.f),
            3,
            0,
            0.5f,
            fighterBullet,
            fighterDestruction,
            false
        );

    }

private:
    static std::shared_ptr<SpriteComposite> createPlayerSprite() {
        auto comp = std::make_shared<SpriteComposite>();
        auto base1 = std::make_shared<SpriteWrapper>("assets/player/ms1.png");
        auto engineBase = std::make_shared<SpriteWrapper>("assets/player/engineBase.png");
        auto engineIdle = std::make_shared<SpriteWrapper>("assets/player/engineIdle.png");
        auto enginePowering = std::make_shared<SpriteWrapper>("assets/player/enginePowering.png");
        auto canon = std::make_shared<SpriteWrapper>("assets/player/canon.png");

        std::vector<Frame> engineIdleFrames;
        for (int i = 0; i < 7; i++)
            engineIdleFrames.push_back(Frame{ sf::IntRect({i * 48, 0}, {48, 48}) });
        auto engineIdleAnim = std::make_shared<Animation>(engineIdleFrames, 0.1f);

        std::vector<Frame> enginePoweringFrames;
        for (int i = 0; i < 7; i++)
            enginePoweringFrames.push_back(Frame{ sf::IntRect({i * 48, 0}, {48, 48}) });
        auto enginePoweringAnim = std::make_shared<Animation>(enginePoweringFrames, 0.1f);

        std::vector<Frame> canonFrames;
        for (int i = 0; i < 7; i++)
            canonFrames.push_back(Frame{ sf::IntRect({i * 48, 0}, {48, 48}) });
        auto canonAnim = std::make_shared<Animation>(canonFrames, 0.05f);

        comp->addChild(engineIdle, engineIdleAnim, { 0.f, 0.f });
        comp->addChild(enginePowering, enginePoweringAnim, { 0.f, 0.f });
        comp->addChild(canon, canonAnim, { 0.f, 0.f });
        comp->addChild(engineBase, nullptr, { 0.f, 0.f });
        comp->addChild(base1, nullptr, { 0.f, 0.f });

        comp->setAnimationActive(1, false);
        comp->setVisible(1, false);
        comp->setAnimationActive(2, false);
        return comp;
    }

    static std::shared_ptr<SpriteComposite> createPlayerBulletSprite() {
        auto comp = std::make_shared<SpriteComposite>();
        auto spr = std::make_shared<SpriteWrapper>("assets/player/bullet.png");

        std::vector<Frame> frames;
        for (int i = 0; i < 4; i++)
            frames.push_back(Frame{ sf::IntRect({i * 32, 0}, {32, 32}) });
        auto anim = std::make_shared<Animation>(frames, 0.1f);

        comp->addChild(spr, anim, { 0.f, 0.f });
        return comp;
    }

    static std::shared_ptr<SpriteComposite> createFighterSprite() {
        auto comp = std::make_shared<SpriteComposite>();
        auto main = std::make_shared<SpriteWrapper>("assets/ennemies/ship/fighter.png");
        auto engine = std::make_shared<SpriteWrapper>("assets/ennemies/engine/fighter.png");
        auto weapon = std::make_shared<SpriteWrapper>("assets/ennemies/weapon/fighter.png");
        auto destruction = std::make_shared<SpriteWrapper>("assets/ennemies/destruction/fighter.png");

        std::vector<Frame> engineFrames;
        for (int i = 0; i < 8; i++)
            engineFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
        auto engineAnim = std::make_shared<Animation>(engineFrames, 0.1f);

        std::vector<Frame> weaponFrames;
        for (int i = 0; i < 9; i++)
            weaponFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
        auto weaponAnim = std::make_shared<Animation>(weaponFrames, 0.1f);

        std::vector<Frame> destructionFrames;
        for (int i = 0; i < 9; i++)
            destructionFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
        auto destructionAnim = std::make_shared<Animation>(destructionFrames, 0.1f);

        comp->addChild(main, nullptr, { 0.f, 0.f });
        comp->addChild(engine, engineAnim, { 0.f, 0.f });
        comp->addChild(weapon, weaponAnim, { 0.f, 0.f });
        comp->addChild(destruction, destructionAnim, { 0.f, 0.f });
        comp->setVisible(3, false);
        comp->setAnimationActive(3, false);
        comp->flipY = true;
        return comp;
    }

    static std::shared_ptr<SpriteComposite> createFighterDestructionSprite() {
        auto comp = std::make_shared<SpriteComposite>();
        auto destruction = std::make_shared<SpriteWrapper>("assets/ennemies/destruction/fighter.png");

        std::vector<Frame> destructionFrames;
        for (int i = 0; i < 9; i++)
            destructionFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
        auto destructionAnim = std::make_shared<Animation>(destructionFrames, 0.1f);

        comp->addChild(destruction, destructionAnim, { 0.f, 0.f });
        comp->flipY = true;
        return comp;
    }

    inline static BulletSpawner fighterBulletSpawner = [](Entity& e, Pool& p, float dt) {
        p.spawn(e.getPosition() + sf::Vector2f(27.5f, 32.f));
    };
    

    static std::shared_ptr<SpriteComposite> createFighterBulletSprite() {
        auto comp = std::make_shared<SpriteComposite>();
        auto spr = std::make_shared<SpriteWrapper>("assets/ennemies/projectile/bullet.png");

        std::vector<Frame> frames;
        for (int i = 0; i < 8; i++)
            frames.push_back(Frame{ sf::IntRect({i * 9, 0}, {9, 12}) });
        auto anim = std::make_shared<Animation>(frames, 0.1f);

        comp->addChild(spr, anim, { 0.f, 0.f });
        comp->flipY = true;
        return comp;
    }
};