#include "sprite.hpp"

class ProjectileManager {
public:
    void addProjectile(const sf::Vector2f& pos, std::shared_ptr<SpriteWrapper> sprite, std::shared_ptr<Animation> anim) {
        SpriteComposite proj;
        proj.addChild(sprite, anim, { 0.f, 0.f });
        proj.setPosition(pos);
        m_projectiles.push_back(std::move(proj));
    }

    void update(float dt) {
        for (auto& p : m_projectiles) {
            p.move({ 0.f, -400.f * dt }); // monte vers le haut
            p.update(dt);
        }

        // Supprimer ceux qui sortent de l'écran
        m_projectiles.erase(
            std::remove_if(m_projectiles.begin(), m_projectiles.end(),
                [](const SpriteComposite& p) {
                    return p.getPosition().y < -50.f; // au-dessus de l'écran
                }),
            m_projectiles.end()
        );
    }

    void draw(sf::RenderWindow& window) {
        for (auto& p : m_projectiles) {
            window.draw(p);
        }
    }

private:
    std::vector<SpriteComposite> m_projectiles;
};
