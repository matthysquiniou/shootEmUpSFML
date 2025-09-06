#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "SpriteComposite.hpp"
#include "MovementPatterns.hpp"

class Pool;

class Entity : public sf::Drawable {
public:
    using BulletSpawner = std::function<void(Entity&, Pool&, float)>;

    enum class Type {
        Player,
        Enemy
    };

    explicit Entity(Type type);
    virtual ~Entity();

    void setComposite(SpriteComposite composite);
    void setHitbox(const sf::Vector2f& size, const sf::Vector2f& offset = { 0.f, 0.f });
    void setHurtbox(const sf::Vector2f& size, const sf::Vector2f& offset = { 0.f, 0.f });

    void setMovementPattern(MovementPattern pattern);
    void setPatternState(PatternState pattern);
    void setDestructionPool(std::shared_ptr<Pool> pool);
    std::shared_ptr<Pool> getDestructionPool();

    void setBulletSpawner(BulletSpawner bulletSpawner);
    void setFireRate(float fr);
    void setBulletPool(std::shared_ptr<Pool> pool);

    void update(float dt);

    void move(const sf::Vector2f& offset);
    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;

    void setDesactivateAfterAnimation(bool desactivateAfterAnimation);

    const sf::FloatRect& getHitbox() const;
    const sf::FloatRect& getHurtbox() const;

    SpriteComposite& getComposite();
    Type getType() const;

    int getHealth() const;
    void setHealth(int hp);
    void takeDamage(int dmg);
    void setDamage(int dmg);
    int getDamage();

    bool hurtBy(const Entity& proj) const;

    bool isActive();
    void activate();
    void deactivate();

private:
    std::shared_ptr<Pool> bulletPool;
    std::shared_ptr<Pool> destrcutionPool;
    SpriteComposite m_composite;

    sf::FloatRect m_hitbox{};
    sf::Vector2f m_hitboxOffset{ 0.f, 0.f };

    sf::FloatRect m_hurtbox{};
    sf::Vector2f m_hurtboxOffset{ 0.f, 0.f };

    sf::Vector2f velocity;
    MovementPattern m_pattern;
    PatternState m_patternState;
    BulletSpawner m_bulletSpawner;

    float lastFire = 0.f;
    float m_fireRate = 0.f;

    Type m_type;
    int m_health = 1;
    int m_damage = 0;
    bool actived = false;
    bool m_desactivateAfterAnimation = false;

    inline static std::vector<Entity*> s_players{};
    inline static std::vector<Entity*> s_enemies{};

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
