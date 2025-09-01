#include "sprite.hpp"
#include "projectile.hpp"
#include <optional>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Composite Sprite SFML3");
    window.setFramerateLimit(60);

    auto cannonSprite = std::make_shared<SpriteWrapper>("assets/player/cannon.png");
    std::vector<Frame> cannonFrames;
    for (int i = 0; i < 7; i++)
        cannonFrames.push_back(Frame{ sf::IntRect({ i * 48, 0 }, {48, 48}) });
    auto cannonAnim = std::make_shared<Animation>(cannonFrames, 0.05f);

    auto enginePoweringSprite = std::make_shared<SpriteWrapper>("assets/player/enginePowering.png");

    std::vector<Frame> enginePoweringFrames;
    for (int i = 0; i < 4; i++)
        enginePoweringFrames.push_back(Frame{ sf::IntRect({ i * 48, 0 }, {48, 48}) });
    auto enginePoweringAnim = std::make_shared<Animation>(enginePoweringFrames, 0.08f);

    auto engineIdleSprite = std::make_shared<SpriteWrapper>("assets/player/engineIdle.png");

    std::vector<Frame> engineIdleFrames;
    for (int i = 0; i < 4; i++)
        engineIdleFrames.push_back(Frame{ sf::IntRect({ i * 48, 0 }, {48, 48}) });
    auto engineIdleAnim = std::make_shared<Animation>(engineIdleFrames, 0.15f);

    auto shipSprite = std::make_shared<SpriteWrapper>("assets/player/ms1.png");

    auto engineSprite = std::make_shared<SpriteWrapper>("assets/player/engineBase.png");

    SpriteComposite player;
    player.addChild(enginePoweringSprite, enginePoweringAnim, { 0.f, 0.f });
    player.addChild(engineIdleSprite, engineIdleAnim, { 0.f, 0.f });
    player.addChild(cannonSprite, cannonAnim, { 0.f, 0.f });
    player.addChild(engineSprite, nullptr, { 0.f, 0.f });
    player.addChild(shipSprite, nullptr, { 0.f, 0.f });

    player.setAnimationActive(2, false);
    player.setVisible(0, false);

    ProjectileManager projectileManager;
    bool shooting = false;
    float timeSinceLastShot = 0.f;
    const float fireRate = 0.2f;

    sf::Clock clock;
    while (window.isOpen()) {

        float dt = clock.restart().asSeconds();
        timeSinceLastShot += dt;

        sf::Vector2f velocity(0.f, 0.f);

        while (auto e = window.pollEvent()) {
            if (e->is<sf::Event::Closed>())
                window.close();

            if (auto pressed = e->getIf<sf::Event::MouseButtonPressed>()) {
                if (pressed->button == sf::Mouse::Button::Left) {
                    player.setAnimationActive(2, true); 
                    shooting = true;
                }
            }

            if (auto released = e->getIf<sf::Event::MouseButtonReleased>()) {
                if (released->button == sf::Mouse::Button::Left) {
                    player.stopAnimationAfterLoop(2);
                    shooting = false;
                }
            }
        }

        if (shooting && timeSinceLastShot >= fireRate) {
            timeSinceLastShot = 0.f;

            auto projSprite = std::make_shared<SpriteWrapper>("assets/player/bullet.png");
            std::vector<Frame> projFrames;
            for (int i = 0; i < 4; i++)
                projFrames.push_back(Frame{ sf::IntRect({ i * 32, 0 }, {32, 32}) });
            auto projAnim = std::make_shared<Animation>(projFrames, 0.1f);

            sf::Vector2f projPos = player.getPosition() + sf::Vector2f(8.f, -20.f);
            projectileManager.addProjectile(projPos, projSprite, projAnim);
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            velocity.x -= 200.f * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            velocity.x += 200.f * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            velocity.y -= 200.f * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            velocity.y += 200.f * dt;
        }

        if (velocity != sf::Vector2f{ 0.f,0.f }) {
            player.setVisible(1, false); 
            player.setVisible(0, true);  
        }
        else {
            player.setVisible(1, true);
            player.setVisible(0, false);
        }

        player.move(velocity);
        player.update(dt);

        projectileManager.update(dt);

        window.clear();
        window.draw(player);
        projectileManager.draw(window);
        window.display();
    }
}
