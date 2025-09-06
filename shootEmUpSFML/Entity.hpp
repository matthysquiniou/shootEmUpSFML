#pragma once
#include "SpriteComposite.hpp"
#include <functional>
#include <vector>
#include <optional>
#include <iostream>

class Pool;

class Entity : public sf::Drawable {
public:
    using MovementPattern = std::function<void(Entity&, float)>;
    using BulletSpawner = std::function<void(Entity&, Pool&, float)>;

    enum class Type {
        Player,
        Enemy
    };

    Entity(Type type)
        : m_type(type)
    {}

    virtual ~Entity() {}

    void setComposite(SpriteComposite composite) {
        m_composite = std::move(composite);
    }

    void setHitbox(const sf::Vector2f& size, const sf::Vector2f& offset = { 0.f,0.f }) {
        m_hitbox.size = size;
        m_hitboxOffset = offset;
    }

    void setHurtbox(const sf::Vector2f& size, const sf::Vector2f& offset = { 0.f,0.f }) {
        m_hurtbox.size = size;
        m_hurtboxOffset = offset;
    }

    void setMovementPattern(MovementPattern pattern) {
        m_pattern = std::move(pattern);
    }

    void setDestructionPool(std::shared_ptr<Pool> pool) {
        destrcutionPool = pool;
    }

    std::shared_ptr<Pool> getDestructionPool() {
        return destrcutionPool;
    }

    void setBulletSpawner(BulletSpawner bulletSpawner) {
        m_bulletSpawner = bulletSpawner;
    }

    void setFireRate(float fr) {
        m_fireRate = fr;
    }

    void setBulletPool(std::shared_ptr<Pool> pool) {
        bulletPool = pool;
    }

    void update(float dt) {
        if (m_pattern) {
            m_pattern(*this, dt);
        }

        if (m_bulletSpawner) {
            lastFire += dt;
            if (lastFire >= m_fireRate) {
                lastFire = 0.f;
                m_bulletSpawner(*this, *bulletPool, dt);
            }
        }

        if (m_desactivateAfterAnimation && !m_composite.isAnimationGoing())
        {
            actived = false;
        }

        m_hitbox.position = m_composite.getPosition() + m_hitboxOffset;
        m_hurtbox.position = m_composite.getPosition() + m_hurtboxOffset;

        m_composite.update(dt);
    }

    void move(const sf::Vector2f& offset) {
        m_composite.move(offset);
    }

    void setPosition(const sf::Vector2f& pos) {
        m_composite.setPosition(pos);
    }

    void setDesactivateAfterAnimation(bool desactivateAfterAnimation) {
        m_desactivateAfterAnimation = desactivateAfterAnimation;
    }

    sf::Vector2f getPosition() const {
        return m_composite.getPosition();
    }

    const sf::FloatRect& getHitbox() const { return m_hitbox; }
    const sf::FloatRect& getHurtbox() const { return m_hurtbox; }

    SpriteComposite& getComposite() { return m_composite; }
    Type getType() const { return m_type; }

    int getHealth() const { return m_health; }
    void setHealth(int hp) { m_health = hp; }
    void takeDamage(int dmg) { m_health -= dmg; }
    void setDamage(int dmg) { m_damage = dmg; }
    int getDamage() { return m_damage; }

    bool hurtBy(const Entity& proj) const {
        return m_hitbox.findIntersection(proj.m_hurtbox).has_value();
    }

    bool isActive() { return actived; }
    void activate() { actived = true; }
    void deactivate() { actived = false; }

private:
    std::shared_ptr<Pool> bulletPool;
    std::shared_ptr<Pool> destrcutionPool;
    SpriteComposite m_composite;
    sf::FloatRect m_hitbox{};
    sf::Vector2f m_hitboxOffset{ 0.f,0.f };

    sf::FloatRect m_hurtbox{};
    sf::Vector2f m_hurtboxOffset{ 0.f,0.f };

    sf::Vector2f velocity;
    MovementPattern m_pattern;

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

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_composite, states);
    }
};
