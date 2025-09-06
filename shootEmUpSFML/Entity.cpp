#include "Entity.hpp"
#include "Pool.hpp"
#include "SoundManager.hpp"
#include <iostream>

// --- ctor/dtor ---
Entity::Entity(Type type) : m_type(type) {}
Entity::~Entity() = default;

// --- setters ---
void Entity::setComposite(SpriteComposite composite) {
    m_composite = std::move(composite);
}

void Entity::setHitbox(const sf::Vector2f& size, const sf::Vector2f& offset) {
    m_hitbox.size = size;
    m_hitboxOffset = offset;
}

void Entity::setHurtbox(const sf::Vector2f& size, const sf::Vector2f& offset) {
    m_hurtbox.size = size;
    m_hurtboxOffset = offset;
}

void Entity::setMovementPattern(MovementPattern pattern) {
    m_pattern = std::move(pattern);
}

void Entity::setDestructionPool(std::shared_ptr<Pool> pool) {
    destrcutionPool = pool;
}

std::shared_ptr<Pool> Entity::getDestructionPool() {
    return destrcutionPool;
}

void Entity::setBulletSpawner(BulletSpawner bulletSpawner) {
    m_bulletSpawner = bulletSpawner;
}

void Entity::setFireRate(float fr) {
    m_fireRate = fr;
}

void Entity::setBulletPool(std::shared_ptr<Pool> pool) {
    bulletPool = pool;
}

// --- update ---
void Entity::update(float dt) {
    if (m_pattern) {
        m_pattern(*this, dt);
    }

    if (m_bulletSpawner) {
        lastFire += dt;
        if (lastFire >= m_fireRate) {
            lastFire = 0.f;
            m_bulletSpawner(*this, *bulletPool, dt);
            SoundManager::playSwoosh();
        }
    }

    if (m_desactivateAfterAnimation && !m_composite.isAnimationGoing()) {
        actived = false;
    }

    m_hitbox.position = m_composite.getPosition() + m_hitboxOffset;
    m_hurtbox.position = m_composite.getPosition() + m_hurtboxOffset;

    m_composite.update(dt);
}

// --- transforms ---
void Entity::move(const sf::Vector2f& offset) {
    m_composite.move(offset);
}

void Entity::setPosition(const sf::Vector2f& pos) {
    m_composite.setPosition(pos);
}

sf::Vector2f Entity::getPosition() const {
    return m_composite.getPosition();
}

void Entity::setDesactivateAfterAnimation(bool desactivateAfterAnimation) {
    m_desactivateAfterAnimation = desactivateAfterAnimation;
}

// --- getters ---
const sf::FloatRect& Entity::getHitbox() const { return m_hitbox; }
const sf::FloatRect& Entity::getHurtbox() const { return m_hurtbox; }

SpriteComposite& Entity::getComposite() { return m_composite; }
Entity::Type Entity::getType() const { return m_type; }

int Entity::getHealth() const { return m_health; }
void Entity::setHealth(int hp) { m_health = hp; }
void Entity::takeDamage(int dmg) { m_health -= dmg; }
void Entity::setDamage(int dmg) { m_damage = dmg; }
int Entity::getDamage() { return m_damage; }

bool Entity::hurtBy(const Entity& proj) const {
    return m_hitbox.findIntersection(proj.m_hurtbox).has_value();
}

// --- activity ---
bool Entity::isActive() { return actived; }
void Entity::activate() { actived = true; }
void Entity::deactivate() { actived = false; }

// --- draw ---
void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_composite, states);
}
