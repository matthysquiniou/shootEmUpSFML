#include "SpriteComposite.hpp"
#include "Entity.hpp"
#include "Pool.hpp"
#include "ColisionManager.hpp"
#include "SoundManager.hpp"
#include "BackgroundManager.hpp"
#include "EnemySpawner.hpp"
#include "ScoreManager.hpp"
#include "GameState.hpp"
#include "MenuManager.hpp"
#include "GameOverScreen.hpp"
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
    bool displayBox = false;
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

    std::shared_ptr<Entity> player;

    auto colisionManager = ColisionManager(pools);

    bool shooting = false;
    float timeSinceLastShot = 0.f;
    const float fireRate = 0.2f;
    float minX = 10.f, maxX = 1250.f;
    float minY = 10.f, maxY = 690.f;

    sf::Clock clock;

    GameState state = GameState::Menu;
    MenuManager menu;
    GameOverScreen gameOver;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (auto event = window.pollEvent()) {
            const sf::Event& e = *event;
            if (e.is<sf::Event::Closed>()) {
                window.close();
            }
            if (state == GameState::Menu) {
                menu.handleEvent(e, state);
                if (state == GameState::Playing) {
                    pools = PoolManager();
                    colisionManager = ColisionManager(pools);
                    spawner = EnemySpawner(pools);
                    spawner.addEnemyType(10, [&](sf::Vector2f pos) { pools.fighter.spawn(pos); });
                    spawner.addEnemyType(10, [&](sf::Vector2f pos) { pools.scout.spawn(pos); });
                    spawner.addEnemyType(15, [&](sf::Vector2f pos) { pools.bomber.spawn(pos); });
                    spawner.addEnemyType(15, [&](sf::Vector2f pos) { pools.torpedo.spawn(pos); });
                    spawner.addEnemyType(20, [&](sf::Vector2f pos) { pools.frigate.spawn(pos); });
                    spawner.addEnemyType(30, [&](sf::Vector2f pos) { pools.battleCruiser.spawn(pos); });

                    player = pools.player.spawn({ 400.f, 500.f });
                    shooting = false;
                }
            }
            else if (state == GameState::GameOver) {
                gameOver.handleEvent(e, state);
                if (state == GameState::Playing) {
                    pools = PoolManager();
                    colisionManager = ColisionManager(pools);
                    spawner = EnemySpawner(pools);
                    spawner.addEnemyType(10, [&](sf::Vector2f pos) { pools.fighter.spawn(pos); });
                    spawner.addEnemyType(10, [&](sf::Vector2f pos) { pools.scout.spawn(pos); });
                    spawner.addEnemyType(15, [&](sf::Vector2f pos) { pools.bomber.spawn(pos); });
                    spawner.addEnemyType(15, [&](sf::Vector2f pos) { pools.torpedo.spawn(pos); });
                    spawner.addEnemyType(20, [&](sf::Vector2f pos) { pools.frigate.spawn(pos); });
                    spawner.addEnemyType(30, [&](sf::Vector2f pos) { pools.battleCruiser.spawn(pos); });

                    player = pools.player.spawn({ 400.f, 500.f });
                    shooting = false;
                }
            }
            else {
                if (auto pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (pressed->button == sf::Mouse::Button::Left) {
                        shooting = true;
                        auto& playerComp = player->getComposite();
                        playerComp.setAnimationActive(2, true);
                    }
                }
                if (auto released = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (released->button == sf::Mouse::Button::Left) {
                        shooting = false;
                        auto& playerComp = player->getComposite();
                        playerComp.stopAnimationAfterLoop(2);
                    }
                }
            }
        }

        window.clear();

        if (state == GameState::Menu) {
            menu.draw(window);
        }
        else if (state == GameState::Playing) {
            spawner.update(dt);
            timeSinceLastShot += dt;

            sf::Vector2f velocity(0.f, 0.f);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  velocity.x -= 400.f * dt;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) velocity.x += 400.f * dt;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    velocity.y -= 400.f * dt;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  velocity.y += 400.f * dt;
            auto playerPos = player->getPosition();

            if (playerPos.x <= minX && velocity.x < 0) {     
                velocity.x = 0;                     
            }
            else if (playerPos.x >= maxX && velocity.x > 0) { 
                velocity.x = 0;                     
            }

            if (playerPos.y <= minY && velocity.y < 0) {      
                velocity.y = 0;                    
            }
            else if (playerPos.y >= maxY && velocity.y > 0) { 
                velocity.y = 0;                   
            }

            if (velocity.x == 0 && velocity.y == 0) {
                auto& playerComp = player->getComposite();
                playerComp.setAnimationActive(1, false);
                playerComp.setVisible(1, false);
                playerComp.setAnimationActive(0, true);
                playerComp.setVisible(0, true);
            }else {
                auto& playerComp = player->getComposite();
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

            window.draw(bgManager);
            ScoreManager::draw(window);

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

            if (displayBox) {
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

            if (!player || !player->isActive()) {
                state = GameState::GameOver;
            }
        }
        else if (state == GameState::GameOver) {
            gameOver.draw(window, ScoreManager::getScore());
        }

        window.display();
    }
}
