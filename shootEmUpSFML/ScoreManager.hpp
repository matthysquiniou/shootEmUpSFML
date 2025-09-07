#pragma once
#include <SFML/Graphics.hpp>

class ScoreManager {
public:
    static void addScore(int points);

    static int getScore();

    static void reset();

    static void draw(sf::RenderTarget& target);

private:
    static int s_score;               
    static sf::Font s_font;            
    static sf::Text s_text;         
    static bool s_initialized;         

    static void init();
};
