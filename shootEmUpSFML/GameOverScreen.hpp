#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.hpp"

class GameOverScreen {
public:
    GameOverScreen();

    void handleEvent(const sf::Event& event, GameState& state);
    void draw(sf::RenderWindow& window, int finalScore);

private:
    sf::Font m_font;
    sf::Text m_gameOverText = sf::Text(m_font);
    sf::Text m_retryText = sf::Text(m_font);
    sf::Text m_exitText = sf::Text(m_font);
};
