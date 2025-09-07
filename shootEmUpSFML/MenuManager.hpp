#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.hpp"

class MenuManager {
public:
    MenuManager();

    void handleEvent(const sf::Event& event, GameState& state);

    void draw(sf::RenderWindow& window);

private:
    sf::Font m_font;
    sf::Text m_title = sf::Text(m_font);
    sf::Text m_playText = sf::Text(m_font);
    sf::Text m_exitText = sf::Text(m_font);
};
