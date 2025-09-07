#include "ColisionManager.hpp"
#include "SoundManager.hpp"
#include "ScoreManager.hpp"

void ColisionManager::update() {

    std::vector<Pool*> enemyPools = {
        &pools.fighter,
        &pools.scout,
        &pools.frigate,
        &pools.torpedo,
        &pools.bomber,
        &pools.battleCruiser
    };

    std::vector<std::shared_ptr<Pool>*> enemybulletPools = {
        &pools.fighterBullet,
        &pools.scoutBullet,
        &pools.frigateBullet,
        &pools.torpedoBullet,
        &pools.bomberBullet,
        &pools.battleCruiserBullet
    };

    for (auto pool : enemyPools) {
        for (auto& enemy : pool->getPool()) {
            if (!enemy->isActive()) continue;
            for (auto& proj : pools.playerBullet.getPool()) {
                if (proj->isActive() && enemy->hurtBy(*proj)) {
                    proj->deactivate();
                    enemy->takeDamage(proj->getDamage());
                    if (enemy->getHealth() <= 0) {
                        enemy->deactivate();

                        auto destructionPool = enemy->getDestructionPool();
                        auto entity = destructionPool->spawn(enemy->getPosition());
                        entity->getComposite().stopAnimationAfterLoop(0, true);
                        SoundManager::playDestruction();
                        ScoreManager::addScore(enemy->getScore());
                        continue;
                    }
                    SoundManager::playHit();
                }
            }
        }
    }

    for (auto& player : pools.player.getPool()) {
        if (player->isActive()) {
            for (auto pool : enemybulletPools) {
                for (auto& proj : (*pool)->getPool()) {
                    if (proj->isActive() && player->hurtBy(*proj)) {
                        proj->deactivate();
                        player->takeDamage(proj->getDamage());
                        changePlayerSprite(player);
                        if (player->getHealth() <= 0) {
                            player->deactivate();
                            // TODO game over screen
                            SoundManager::playDestruction();
                            continue;
                        }
                        SoundManager::playHit();
                    }
                }
            }
        }
    }
}

void ColisionManager::changePlayerSprite(std::shared_ptr<Entity> player) {
    auto& comp = player->getComposite();
    float healthPercent = player->getHealthPercent();
    if (healthPercent <= 0.25f)
    {
        comp.setVisible(6, false);
        comp.setVisible(7, true);
        return;
    }
    if (healthPercent <= 0.5f)
    {
        comp.setVisible(5, false);
        comp.setVisible(6, true);
        return;
    }
    if (healthPercent <= 0.75f)
    {
        comp.setVisible(4, false);
        comp.setVisible(5, true);
        return;
    }
}