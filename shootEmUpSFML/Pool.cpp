#include "Pool.hpp"

// ===================== Pool =====================

Pool::Pool(
    std::size_t capacity,
    Entity::Type entityType,
    std::shared_ptr<SpriteComposite> sprite,
    MovementPattern pattern,
    BulletSpawner bulletSpawner,
    sf::Vector2f hitbox,
    sf::Vector2f hurtbox,
    sf::Vector2f boxOffSet,
    int health,
    int damage,
    float fireRate,
    std::shared_ptr<Pool> bulletPool,
    std::shared_ptr<Pool> destructionPool,
    bool desactivateAfterAnimation,
    PatternState patternState
)
    : sprite(sprite),
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
    desactivateAfterAnimation(desactivateAfterAnimation),
    patternState(patternState)
{
    pool.reserve(capacity);
    for (std::size_t i = 0; i < capacity; i++) {
        pool.push_back(std::make_shared<Entity>(entityType));
    }
}

std::shared_ptr<Entity> Pool::spawn(const sf::Vector2f& pos, std::shared_ptr<PatternState> ps) {
    for (auto& obj : pool) {
        if (!obj->isActive()) {
            auto comp = *sprite;
            obj->setComposite(comp);
            obj->setHurtbox(hurtbox, boxOffSet);
            obj->setHitbox(hitbox, boxOffSet);
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
            if (ps != nullptr) {
                obj->setPatternState(*ps);
            } else {
                obj->setPatternState(patternState);
            }
            return obj;
        }
    }
    return nullptr;
}

void Pool::update(float dt) {
    for (auto& obj : pool) {
        if (obj->isActive()) {
            obj->update(dt);

            if (obj->getPosition().y < -100.f || obj->getPosition().y > 700.f ||
                obj->getPosition().x < -100.f || obj->getPosition().x > 900.f) {
                obj->deactivate();
            }
        }
    }
}

void Pool::draw(sf::RenderWindow& window) {
    for (auto& obj : pool) {
        if (obj->isActive()) {
            window.draw(*obj);
        }
    }
}

const std::vector<std::shared_ptr<Entity>>& Pool::getPool() const {
    return pool;
}

// ===================== PoolManager =====================

// PLAYER

std::shared_ptr<SpriteComposite> PoolManager::createPlayerSprite() {
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

    std::vector<Frame> enginePoweringFrames; for (int i = 0; i < 7; i++)
        enginePoweringFrames.push_back(Frame{ sf::IntRect({i * 48, 0}, {48, 48}) });
    auto enginePoweringAnim = std::make_shared<Animation>(enginePoweringFrames, 0.07f);
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

std::shared_ptr<SpriteComposite> PoolManager::createPlayerBulletSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto spr = std::make_shared<SpriteWrapper>("assets/player/bullet.png");

    std::vector<Frame> frames;
    for (int i = 0; i < 4; i++)
        frames.push_back(Frame{ sf::IntRect({i * 32, 0}, {32, 32}) });
    auto anim = std::make_shared<Animation>(frames, 0.1f);

    comp->addChild(spr, anim, { 0.f, 0.f });
    return comp;
}

// FIGHTER

PoolManager::BulletSpawner PoolManager::fighterBulletSpawner = [](Entity& e, Pool& p, float dt) {
    p.spawn(e.getPosition() + sf::Vector2f(27.5f, 32.f));
};

std::shared_ptr<SpriteComposite> PoolManager::createFighterSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto main = std::make_shared<SpriteWrapper>("assets/ennemies/ship/fighter.png");
    auto engine = std::make_shared<SpriteWrapper>("assets/ennemies/engine/fighter.png");
    auto weapon = std::make_shared<SpriteWrapper>("assets/ennemies/weapon/fighter.png");

    std::vector<Frame> engineFrames;
    for (int i = 0; i < 8; i++)
        engineFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto engineAnim = std::make_shared<Animation>(engineFrames, 0.1f);

    std::vector<Frame> weaponFrames;
    for (int i = 0; i < 9; i++)
        weaponFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto weaponAnim = std::make_shared<Animation>(weaponFrames, 0.1f);

    comp->addChild(main, nullptr, { 0.f, 0.f });
    comp->addChild(engine, engineAnim, { 0.f, 0.f });
    comp->addChild(weapon, weaponAnim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

std::shared_ptr<SpriteComposite> PoolManager::createFighterBulletSprite() {
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

std::shared_ptr<SpriteComposite> PoolManager::createFighterDestructionSprite() {
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

// SCOUT

PoolManager::BulletSpawner PoolManager::scoutBulletSpawner = [](Entity& e, Pool& p, float dt) {
    p.spawn(e.getPosition() + sf::Vector2f(27.5f, 32.f));
};

std::shared_ptr<SpriteComposite> PoolManager::createScoutSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto main = std::make_shared<SpriteWrapper>("assets/ennemies/ship/scout.png");
    auto engine = std::make_shared<SpriteWrapper>("assets/ennemies/engine/scout.png");
    auto weapon = std::make_shared<SpriteWrapper>("assets/ennemies/weapon/scout.png");

    std::vector<Frame> engineFrames;
    for (int i = 0; i < 8; i++)
        engineFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto engineAnim = std::make_shared<Animation>(engineFrames, 0.1f);

    std::vector<Frame> weaponFrames;
    for (int i = 0; i < 7; i++)
        weaponFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto weaponAnim = std::make_shared<Animation>(weaponFrames, 0.1f);

    comp->addChild(main, nullptr, { 0.f, 0.f });
    comp->addChild(engine, engineAnim, { 0.f, 0.f });
    comp->addChild(weapon, weaponAnim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

std::shared_ptr<SpriteComposite> PoolManager::createScoutBulletSprite() {
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

std::shared_ptr<SpriteComposite> PoolManager::createScoutDestructionSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto destruction = std::make_shared<SpriteWrapper>("assets/ennemies/destruction/scout.png");

    std::vector<Frame> destructionFrames;
    for (int i = 0; i < 9; i++)
        destructionFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto destructionAnim = std::make_shared<Animation>(destructionFrames, 0.1f);

    comp->addChild(destruction, destructionAnim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

// FRIGATE

PoolManager::BulletSpawner PoolManager::frigateBulletSpawner = [](Entity& e, Pool& p, float dt) {
    p.spawn(e.getPosition() + sf::Vector2f(0.f, 32.f));
};


std::shared_ptr<SpriteComposite> PoolManager::createFrigateSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto main = std::make_shared<SpriteWrapper>("assets/ennemies/ship/frigate.png");
    auto engine = std::make_shared<SpriteWrapper>("assets/ennemies/engine/frigate.png");
    auto weapon = std::make_shared<SpriteWrapper>("assets/ennemies/weapon/frigate.png");

    std::vector<Frame> engineFrames;
    for (int i = 0; i < 8; i++)
        engineFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto engineAnim = std::make_shared<Animation>(engineFrames, 0.1f);

    std::vector<Frame> weaponFrames;
    for (int i = 0; i < 9; i++)
        weaponFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto weaponAnim = std::make_shared<Animation>(weaponFrames, 0.1f);

    comp->addChild(main, nullptr, { 0.f, 0.f });
    comp->addChild(engine, engineAnim, { 0.f, 0.f });
    comp->addChild(weapon, weaponAnim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

std::shared_ptr<SpriteComposite> PoolManager::createFrigateBulletSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto spr = std::make_shared<SpriteWrapper>("assets/ennemies/projectile/wave.png");

    std::vector<Frame> frames;
    for (int i = 0; i < 6; i++)
        frames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto anim = std::make_shared<Animation>(frames, 0.1f);

    comp->addChild(spr, anim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

std::shared_ptr<SpriteComposite> PoolManager::createFrigateDestructionSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto destruction = std::make_shared<SpriteWrapper>("assets/ennemies/destruction/frigate.png");

    std::vector<Frame> destructionFrames;
    for (int i = 0; i < 9; i++)
        destructionFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto destructionAnim = std::make_shared<Animation>(destructionFrames, 0.1f);

    comp->addChild(destruction, destructionAnim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

// TORPEDO

PoolManager::BulletSpawner PoolManager::torpedoBulletSpawner = [](Entity& e, Pool& p, float dt) {
    p.spawn(e.getPosition() + sf::Vector2f(24.f, 32.f));
};

std::shared_ptr<SpriteComposite> PoolManager::createTorpedoSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto main = std::make_shared<SpriteWrapper>("assets/ennemies/ship/torpedo.png");
    auto engine = std::make_shared<SpriteWrapper>("assets/ennemies/engine/torpedo.png");
    auto weapon = std::make_shared<SpriteWrapper>("assets/ennemies/weapon/torpedo.png");

    std::vector<Frame> engineFrames;
    for (int i = 0; i < 8; i++)
        engineFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto engineAnim = std::make_shared<Animation>(engineFrames, 0.1f);

    std::vector<Frame> weaponFrames;
    for (int i = 0; i < 16; i++)
        weaponFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto weaponAnim = std::make_shared<Animation>(weaponFrames, 0.1f);

    comp->addChild(main, nullptr, { 0.f, 0.f });
    comp->addChild(engine, engineAnim, { 0.f, 0.f });
    comp->addChild(weapon, weaponAnim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

std::shared_ptr<SpriteComposite> PoolManager::createTorpedoBulletSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto spr = std::make_shared<SpriteWrapper>("assets/ennemies/projectile/rocket.png");

    std::vector<Frame> frames;
    for (int i = 0; i < 6; i++)
        frames.push_back(Frame{ sf::IntRect({i * 16, 0}, {16, 32}) });
    auto anim = std::make_shared<Animation>(frames, 0.1f);

    comp->addChild(spr, anim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

std::shared_ptr<SpriteComposite> PoolManager::createTorpedoDestructionSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto destruction = std::make_shared<SpriteWrapper>("assets/ennemies/destruction/torpedo.png");

    std::vector<Frame> destructionFrames;
    for (int i = 0; i < 8; i++)
        destructionFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto destructionAnim = std::make_shared<Animation>(destructionFrames, 0.1f);

    comp->addChild(destruction, destructionAnim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

// BOMBER

PoolManager::BulletSpawner PoolManager::bomberBulletSpawner = [](Entity& e, Pool& p, float dt) {
    p.spawn(e.getPosition() + sf::Vector2f(24.f, 32.f));
};

std::shared_ptr<SpriteComposite> PoolManager::createBomberSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto main = std::make_shared<SpriteWrapper>("assets/ennemies/ship/bomber.png");
    auto engine = std::make_shared<SpriteWrapper>("assets/ennemies/engine/bomber.png");

    std::vector<Frame> engineFrames;
    for (int i = 0; i < 8; i++)
        engineFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto engineAnim = std::make_shared<Animation>(engineFrames, 0.1f);

    comp->addChild(main, nullptr, { 0.f, 0.f });
    comp->addChild(engine, engineAnim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

std::shared_ptr<SpriteComposite> PoolManager::createBomberBulletSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto spr = std::make_shared<SpriteWrapper>("assets/ennemies/projectile/bomb.png");

    std::vector<Frame> frames;
    for (int i = 0; i < 16; i++)
        frames.push_back(Frame{ sf::IntRect({i * 16, 0}, {16, 16}) });
    auto anim = std::make_shared<Animation>(frames, 0.1f);

    comp->addChild(spr, anim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

std::shared_ptr<SpriteComposite> PoolManager::createBomberDestructionSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto destruction = std::make_shared<SpriteWrapper>("assets/ennemies/destruction/bomber.png");

    std::vector<Frame> destructionFrames;
    for (int i = 0; i < 10; i++)
        destructionFrames.push_back(Frame{ sf::IntRect({i * 64, 0}, {64, 64}) });
    auto destructionAnim = std::make_shared<Animation>(destructionFrames, 0.1f);

    comp->addChild(destruction, destructionAnim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

// BATTLE CRUISER

PoolManager::BulletSpawner PoolManager::battleCruiserBulletSpawner = [](Entity& e, Pool& p, float dt) {
    p.spawn(e.getPosition() + sf::Vector2f(24.f, 32.f));
};

std::shared_ptr<SpriteComposite> PoolManager::createBattleCruiserSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto main = std::make_shared<SpriteWrapper>("assets/ennemies/ship/battlecruiser.png");
    auto engine = std::make_shared<SpriteWrapper>("assets/ennemies/engine/battlecruiser.png");
    auto weapon = std::make_shared<SpriteWrapper>("assets/ennemies/weapon/battlecruiser.png");

    std::vector<Frame> engineFrames;
    for (int i = 0; i < 8; i++)
        engineFrames.push_back(Frame{ sf::IntRect({i * 128, 0}, {128, 128}) });
    auto engineAnim = std::make_shared<Animation>(engineFrames, 0.1f);

    std::vector<Frame> weaponFrames;
    for (int i = 0; i < 9; i++)
        weaponFrames.push_back(Frame{ sf::IntRect({i * 128, 0}, {128, 128}) });
    auto weaponAnim = std::make_shared<Animation>(weaponFrames, 0.1f);

    comp->addChild(main, nullptr, { 0.f, 0.f });
    comp->addChild(engine, engineAnim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

std::shared_ptr<SpriteComposite> PoolManager::createBattleCruiserBulletSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto spr = std::make_shared<SpriteWrapper>("assets/ennemies/projectile/spinning bullet.png");

    std::vector<Frame> frames;
    for (int i = 0; i < 8; i++)
        frames.push_back(Frame{ sf::IntRect({i * 8, 0}, {8, 8}) });
    auto anim = std::make_shared<Animation>(frames, 0.1f);

    comp->addChild(spr, anim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

std::shared_ptr<SpriteComposite> PoolManager::createBattleCruiserDestructionSprite() {
    auto comp = std::make_shared<SpriteComposite>();
    auto destruction = std::make_shared<SpriteWrapper>("assets/ennemies/destruction/battlecruiser.png");

    std::vector<Frame> destructionFrames;
    for (int i = 0; i < 13; i++)
        destructionFrames.push_back(Frame{ sf::IntRect({i * 128, 0}, {128, 128}) });
    auto destructionAnim = std::make_shared<Animation>(destructionFrames, 0.1f);

    comp->addChild(destruction, destructionAnim, { 0.f, 0.f });
    comp->flipY = true;
    return comp;
}

PoolManager::PoolManager() {

    // PLAYER

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
        false,
        PatternState()
    );

    playerBullet = Pool(
        128,
        Entity::Type::Player,
        createPlayerBulletSprite(),
        MovementPatterns::linear(-400.f),
        nullptr,
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(16.f, 16.f),
        sf::Vector2f(8.f, 8.f),
        1,
        1,
        0.f,
        nullptr,
        nullptr,
        false,
        PatternState()
    );

    // FIGHTER

    fighterBullet = std::make_shared<Pool>(
        128,
        Entity::Type::Enemy,
        createFighterBulletSprite(),
        MovementPatterns::linear(250.f),
        nullptr,
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(9.f, 12.f),
        sf::Vector2f(0.f, 0.f),
        1,
        1,
        0.f,
        nullptr,
        nullptr,
        false,
        PatternState()
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
        true,
        PatternState()
    );

    fighter = Pool(
        32,
        Entity::Type::Enemy,
        createFighterSprite(),
        MovementPatterns::pingPongY(200.f,100.f,500.f),
        fighterBulletSpawner,
        sf::Vector2f(32.f, 32.f),
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(16.f, 16.f),
        3,
        0,
        0.5f,
        fighterBullet,
        fighterDestruction,
        false,
        PatternState()
    );

    // SCOUT

    scoutBullet = std::make_shared<Pool>(
        128,
        Entity::Type::Enemy,
        createScoutBulletSprite(),
        MovementPatterns::linear(250.f),
        nullptr,
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(9.f, 12.f),
        sf::Vector2f(0.f, 0.f),
        1,
        1,
        0.f,
        nullptr,
        nullptr,
        false,
        PatternState()
    );

    scoutDestruction = std::make_shared<Pool>(
        32,
        Entity::Type::Enemy,
        createScoutDestructionSprite(),
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
        true,
        PatternState()
    );

    scout = Pool(
        32,
        Entity::Type::Enemy,
        createScoutSprite(),
        MovementPatterns::pingPongY(200.f, 100.f, 500.f),
        scoutBulletSpawner,
        sf::Vector2f(32.f, 32.f),
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(16.f, 16.f),
        3,
        0,
        0.5f,
        scoutBullet,
        scoutDestruction,
        false,
        PatternState()
    );

    // FRIGATE

    frigateBullet = std::make_shared<Pool>(
        128,
        Entity::Type::Enemy,
        createFrigateBulletSprite(),
        MovementPatterns::linear(250.f),
        nullptr,
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(48.f, 12.f),
        sf::Vector2f(8.f, 24.f),
        1,
        1,
        0.f,
        nullptr,
        nullptr,
        false,
        PatternState()
    );

    frigateDestruction = std::make_shared<Pool>(
        32,
        Entity::Type::Enemy,
        createFrigateDestructionSprite(),
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
        true,
        PatternState()
    );

    frigate = Pool(
        32,
        Entity::Type::Enemy,
        createFrigateSprite(),
        MovementPatterns::pingPongY(200.f, 100.f, 500.f),
        frigateBulletSpawner,
        sf::Vector2f(32.f, 32.f),
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(16.f, 16.f),
        3,
        0,
        0.5f,
        frigateBullet,
        frigateDestruction,
        false,
        PatternState()
    );

    // TORPEDO

    torpedoBullet = std::make_shared<Pool>(
        128,
        Entity::Type::Enemy,
        createTorpedoBulletSprite(),
        MovementPatterns::linear(250.f),
        nullptr,
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(16.f, 32.f),
        sf::Vector2f(0.f, 0.f),
        1,
        1,
        0.f,
        nullptr,
        nullptr,
        false,
        PatternState()
    );

    torpedoDestruction = std::make_shared<Pool>(
        32,
        Entity::Type::Enemy,
        createTorpedoDestructionSprite(),
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
        true,
        PatternState()
    );

    torpedo = Pool(
        32,
        Entity::Type::Enemy,
        createTorpedoSprite(),
        MovementPatterns::pingPongY(200.f, 100.f, 500.f),
        torpedoBulletSpawner,
        sf::Vector2f(32.f, 32.f),
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(16.f, 16.f),
        3,
        0,
        0.5f,
        torpedoBullet,
        torpedoDestruction,
        false,
        PatternState()
    );

    // BOMBER 

    bomberBullet = std::make_shared<Pool>(
        128,
        Entity::Type::Enemy,
        createBomberBulletSprite(),
        MovementPatterns::linear(250.f),
        nullptr,
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(16.f, 16.f),
        sf::Vector2f(0.f, 0.f),
        1,
        1,
        0.f,
        nullptr,
        nullptr,
        false,
        PatternState()
    );

    bomberDestruction = std::make_shared<Pool>(
        32,
        Entity::Type::Enemy,
        createBomberDestructionSprite(),
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
        true,
        PatternState()
    );

    bomber = Pool(
        32,
        Entity::Type::Enemy,
        createBomberSprite(),
        MovementPatterns::pingPongY(200.f, 100.f, 500.f),
        bomberBulletSpawner,
        sf::Vector2f(32.f, 32.f),
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(16.f, 16.f),
        3,
        0,
        0.5f,
        bomberBullet,
        bomberDestruction,
        false,
        PatternState()
    );

    // BATTLE CRUISER 

    battleCruiserBullet = std::make_shared<Pool>(
        128,
        Entity::Type::Enemy,
        createBattleCruiserBulletSprite(),
        MovementPatterns::linear(250.f),
        nullptr,
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(8.f, 8.f),
        sf::Vector2f(0.f, 0.f),
        1,
        1,
        0.f,
        nullptr,
        nullptr,
        false,
        PatternState()
    );

    battleCruiserDestruction = std::make_shared<Pool>(
        32,
        Entity::Type::Enemy,
        createBattleCruiserDestructionSprite(),
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
        true,
        PatternState()
    );

    battleCruiser = Pool(
        32,
        Entity::Type::Enemy,
        createBattleCruiserSprite(),
        MovementPatterns::pingPongY(200.f, 100.f, 500.f),
        battleCruiserBulletSpawner,
        sf::Vector2f(64.f, 64.f),
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(32.f, 32.f),
        3,
        0,
        0.5f,
        battleCruiserBullet,
        battleCruiserDestruction,
        false,
        PatternState()
    );
}
