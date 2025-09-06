#include "SoundManager.hpp"

void SoundManager::init() {
    if (!bufferBackground.loadFromFile("assets/sound/background.mp3"))
        throw std::runtime_error("Impossible de charger background.mp3");
    soundBackground.setVolume(10.f);
    soundBackground.setLooping(true);

    if (!bufferDestruction.loadFromFile("assets/sound/destruction.wav"))
        throw std::runtime_error("Impossible de charger destruction.wav");
    soundDestruction.setVolume(10.f);

    if (!bufferExplosion.loadFromFile("assets/sound/explosion.wav"))
        throw std::runtime_error("Impossible de charger explosion.wav");
    soundExplosion.setVolume(10.f);

    if (!bufferHit.loadFromFile("assets/sound/hit.wav"))
        throw std::runtime_error("Impossible de charger hit.wav");
    soundHit.setVolume(10.f);

    if (!bufferRocket.loadFromFile("assets/sound/rocket.wav"))
        throw std::runtime_error("Impossible de charger rocket.wav");
    soundRocket.setVolume(10.f);

    if (!bufferSwoosh.loadFromFile("assets/sound/swoosh.wav"))
        throw std::runtime_error("Impossible de charger swoosh.wav");
    soundSwoosh.setVolume(75.f);
}

void SoundManager::playBackground() { soundBackground.play(); }
void SoundManager::stopBackground() { soundBackground.stop(); }

void SoundManager::playDestruction() { soundDestruction.play(); }
void SoundManager::playExplosion() { soundExplosion.play(); }
void SoundManager::playHit() { soundHit.play(); }
void SoundManager::playRocket() { soundRocket.play(); }
void SoundManager::playSwoosh() { soundSwoosh.play(); }
