#pragma once
#include "SpriteComposite.hpp"
#include "Entity.hpp"
#include <memory>
#include <vector>
#include <algorithm>

class ProjectileManager {
public:
    void addPlayerProjectile(const sf::Vector2f& pos,
        std::shared_ptr<SpriteWrapper> sprite,
        std::shared_ptr<Animation> anim,
        int damage = 1)
    {
        SpriteComposite projComposite;
        projComposite.addChild(sprite, anim, { 0.f, 0.f });
        projComposite.setPosition(pos);

        auto projectile = std::make_shared<Entity>(Entity::Type::Player, damage);
        projectile->setComposite(projComposite);
        projectile->setHitbox({ 0.f, 0.f });
        projectile->setHurtbox({ 16.f, 16.f },{8.f,8.f});
        projectile->setPosition(pos);

        m_projectiles.push_back(projectile);
    }

    void update(float dt) {
        for (auto& p : m_projectiles) {
            p->move({ 0.f, -400.f * dt }); // monte vers le haut
            p->update(dt);
        }

        // Supprimer les projectiles hors écran
        m_projectiles.erase(
            std::remove_if(m_projectiles.begin(), m_projectiles.end(),
                [](const std::shared_ptr<Entity>& p) {
                    return p->getPosition().y < -50.f;
                }),
            m_projectiles.end()
        );
    }

    void remove(Entity& projectile) {
        projectile.setPosition({ -100.f,-100.f });
    }


    void draw(sf::RenderWindow& window) {
        for (auto& p : m_projectiles) {
            window.draw(*p);
        }
    }

    std::vector<std::shared_ptr<Entity>>& getProjectiles() { return m_projectiles; }

private:
    std::vector<std::shared_ptr<Entity>> m_projectiles;
};
