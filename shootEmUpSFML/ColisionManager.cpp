#include "ColisionManager.hpp"

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

    // Collision joueur -> ennemis
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

                        std::cout << "Enemy destroyed!" << std::endl; // TODO : add score
                    }
                }
            }
        }
    }

    // Collision ennemis -> joueur
    for (auto& player : pools.player.getPool()) {
        if (player->isActive()) {
            for (auto pool : enemybulletPools) {
                for (auto& proj : (*pool)->getPool()) {
                    if (proj->isActive() && player->hurtBy(*proj)) {
                        proj->deactivate();
                        player->takeDamage(proj->getDamage()); // TODO change player visible sprite
                        if (player->getHealth() <= 0) {
                            player->deactivate();
                            std::cout << "you lose" << std::endl; // TODO : game over screen
                        }
                    }
                }
            }
        }
    }
}
