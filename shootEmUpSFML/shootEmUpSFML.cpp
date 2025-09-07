#include "SpriteComposite.hpp"
#include "Entity.hpp"
#include "Pool.hpp"
#include "ColisionManager.hpp"
#include "SoundManager.hpp"
#include "BackgroundManager.hpp"
#include "EnemySpawner.hpp"
#include "ScoreManager.hpp"
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
    bool displayBox = true;
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Space shooter");

    SoundManager::init();
    SoundManager::playBackground();

    BackgroundManager bgManager;
    bgManager.addBackground("assets/background/bg1.png", 9, 2.f, 5.f, 100);
    bgManager.addBackground("assets/background/bg2.png", 9, 0.01f, 25.f, 150);

    ScoreManager::reset();

    window.setFramerateLimit(60);

    PoolManager pools;

    EnemySpawner spawner(pools);

    spawner.addEnemyType(10, [&](sf::Vector2f pos) { pools.fighter.spawn(pos); });
    spawner.addEnemyType(20, [&](sf::Vector2f pos) { pools.scout.spawn(pos); });
    spawner.addEnemyType(20, [&](sf::Vector2f pos) { pools.bomber.spawn(pos); });
    spawner.addEnemyType(20, [&](sf::Vector2f pos) { pools.frigate.spawn(pos); });
    spawner.addEnemyType(20, [&](sf::Vector2f pos) { pools.torpedo.spawn(pos); });
    spawner.addEnemyType(20, [&](sf::Vector2f pos) { pools.battleCruiser.spawn(pos); });

    auto player = pools.player.spawn({ 400.f, 500.f });
    auto& playerComp = player->getComposite();

    auto colisionManager = ColisionManager(pools);

    bool shooting = false;
    float timeSinceLastShot = 0.f;
    const float fireRate = 0.2f;

    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        spawner.update(dt);

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  velocity.x -= 400.f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) velocity.x += 400.f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    velocity.y -= 400.f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  velocity.y += 400.f * dt;

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
            SoundManager::playSwoosh();
        }

        bgManager.update(dt);

        pools.player.update(dt);
        pools.fighter.update(dt);
        pools.scout.update(dt);
        pools.frigate.update(dt);
        pools.torpedo.update(dt);
        pools.bomber.update(dt);
        pools.battleCruiser.update(dt);

        pools.playerBullet.update(dt);
        pools.fighterBullet->update(dt);
        pools.scoutBullet->update(dt);
        pools.frigateBullet->update(dt);
        pools.torpedoBullet->update(dt);
        pools.bomberBullet->update(dt);
        pools.battleCruiserBullet->update(dt);

        colisionManager.update();

        pools.fighterDestruction->update(dt);
        pools.scoutDestruction->update(dt);
        pools.frigateDestruction->update(dt);
        pools.torpedoDestruction->update(dt);
        pools.bomberDestruction->update(dt);
        pools.battleCruiserDestruction->update(dt);

        window.clear();

        ScoreManager::draw(window);

        window.draw(bgManager);

        pools.player.draw(window);
        pools.playerBullet.draw(window);

        pools.fighter.draw(window);
        pools.fighterBullet->draw(window);
        pools.fighterDestruction->draw(window);

        pools.scout.draw(window);
        pools.scoutBullet->draw(window);
        pools.scoutDestruction->draw(window);

        pools.frigate.draw(window);
        pools.frigateBullet->draw(window);
        pools.frigateDestruction->draw(window);

        pools.torpedo.draw(window);
        pools.torpedoBullet->draw(window);
        pools.torpedoDestruction->draw(window);

        pools.bomber.draw(window);
        pools.bomberBullet->draw(window);
        pools.bomberDestruction->draw(window);

        pools.battleCruiser.draw(window);
        pools.battleCruiserBullet->draw(window);
        pools.battleCruiserDestruction->draw(window);


        if (displayBox)
        {
            drawHitboxes(window, pools.player.getPool());
            drawHitboxes(window, pools.playerBullet.getPool());
            drawHitboxes(window, pools.fighter.getPool());
            drawHitboxes(window, pools.fighterBullet->getPool());
            drawHitboxes(window, pools.scout.getPool());
            drawHitboxes(window, pools.scoutBullet->getPool());
            drawHitboxes(window, pools.frigate.getPool());
            drawHitboxes(window, pools.frigateBullet->getPool());
            drawHitboxes(window, pools.torpedo.getPool());
            drawHitboxes(window, pools.torpedoBullet->getPool());
            drawHitboxes(window, pools.bomber.getPool());
            drawHitboxes(window, pools.bomberBullet->getPool());
            drawHitboxes(window, pools.battleCruiser.getPool());
            drawHitboxes(window, pools.battleCruiserBullet->getPool());
        }

        window.display();
    }
}
