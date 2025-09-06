#pragma once
#include <SFML/Audio.hpp>
#include <stdexcept>


class SoundManager {
public:
    static void init();

    static void playBackground();
    static void stopBackground();

    static void playDestruction(); 
    static void playExplosion(); 
    static void playHit(); 
    static void playRocket();
    static void playSwoosh();

private:
    inline static sf::SoundBuffer bufferBackground;
    inline static sf::Sound soundBackground{ bufferBackground };

    inline static sf::SoundBuffer bufferDestruction;
    inline static sf::Sound soundDestruction{ bufferDestruction };

    inline static sf::SoundBuffer bufferExplosion;
    inline static sf::Sound soundExplosion{ bufferExplosion };

    inline static sf::SoundBuffer bufferHit;
    inline static sf::Sound soundHit{ bufferHit };

    inline static sf::SoundBuffer bufferRocket;
    inline static sf::Sound soundRocket{ bufferRocket };

    inline static sf::SoundBuffer bufferSwoosh;
    inline static sf::Sound soundSwoosh{ bufferSwoosh };
};
