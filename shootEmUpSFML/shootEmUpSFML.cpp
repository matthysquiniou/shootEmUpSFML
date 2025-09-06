#include "SpriteComposite.hpp"
#include "Entity.hpp"
#include "Pool.hpp"
#include "BulletManager.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

void drawHitboxes(sf::RenderWindow& window, const std::vector<std::shared_ptr<Entity>>& entities) {
    for (auto& e : entities) {
        if (!e->isActive()) continue;

        sf::RectangleShape hit({ e->getHitbox().size.x, e->getHitbox().size.y });
        hit.setPosition(e->getHitbox().position);
        hit.setFillColor(sf::Color::Transparent);
        hit.setOutlineColor(sf::Color::Red);
        hit.setOutlineThickness(1.f);
        window.draw(hit);

        sf::RectangleShape hurt({ e->getHurtbox().size.x, e->getHurtbox().size.y });
        hurt.setPosition(e->getHurtbox().position);
        hurt.setFillColor(sf::Color::Transparent);
        hurt.setOutlineColor(sf::Color::Blue);
        hurt.setOutlineThickness(1.f);
        window.draw(hurt);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Player vs Enemy");

    sf::SoundBuffer buffer;
    buffer.loadFromFile("assets/sound/background.mp3");

    sf::Sound sound(buffer);
    sound.setVolume(20.f);
    sound.play();

    window.setFramerateLimit(60);

    PoolManager pools;

    auto player = pools.player.spawn({ 400.f, 500.f });
    auto& playerComp = player->getComposite();

    pools.fighter.spawn({ 400.f, 100.f });

    auto bulletManager = BulletManager(pools);

    bool shooting = false;
    float timeSinceLastShot = 0.f;
    const float fireRate = 0.2f;

    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        timeSinceLastShot += dt;

        sf::Vector2f velocity(0.f, 0.f);

        while (auto e = window.pollEvent()) {
            if (e->is<sf::Event::Closed>()) window.close();

            if (auto pressed = e->getIf<sf::Event::MouseButtonPressed>()) {
                if (pressed->button == sf::Mouse::Button::Left) {
                    shooting = true;
                    playerComp.setAnimationActive(2, true);
                }
            }
            if (auto released = e->getIf<sf::Event::MouseButtonReleased>()) {
                if (released->button == sf::Mouse::Button::Left) {
                    shooting = false;
                    playerComp.stopAnimationAfterLoop(2);
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  velocity.x -= 200.f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) velocity.x += 200.f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    velocity.y -= 200.f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  velocity.y += 200.f * dt;

        if (velocity.x == 0 && velocity.y == 0) {
            playerComp.setAnimationActive(1, false);
            playerComp.setVisible(1, false);
            playerComp.setAnimationActive(0, true);
            playerComp.setVisible(0, true);
        }else {
            playerComp.setAnimationActive(1, true);
            playerComp.setVisible(1, true);
            playerComp.setAnimationActive(0, false);
            playerComp.setVisible(0, false);
        }

        if (player) player->move(velocity);


        if (shooting && timeSinceLastShot >= fireRate && player) {
            timeSinceLastShot = 0.f;
            pools.playerBullet.spawn(player->getPosition() + sf::Vector2f(8.f, -20.f));
        }

        bulletManager.update();

        pools.player.update(dt);
        pools.playerBullet.update(dt);
        pools.fighter.update(dt);
        pools.fighterBullet->update(dt);
        pools.fighterDestruction->update(dt);


        window.clear();
        pools.player.draw(window);
        pools.playerBullet.draw(window);
        pools.fighter.draw(window);
        pools.fighterBullet->draw(window);
        pools.fighterDestruction->draw(window);

        drawHitboxes(window, pools.player.getPool());
        drawHitboxes(window, pools.playerBullet.getPool());
        drawHitboxes(window, pools.fighter.getPool());
        drawHitboxes(window, pools.fighterBullet->getPool());

        window.display();
    }
}
