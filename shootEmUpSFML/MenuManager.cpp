#include "MenuManager.hpp"
#include "ScoreManager.hpp"
#include <iostream>

MenuManager::MenuManager() {
    if (!m_font.openFromFile("Tuffy.ttf")) {
        std::cerr << "Erreur: impossible de charger Tuffy.ttf\n";
    }

    m_title.setFont(m_font);
    m_title.setString("Space Shooter");
    m_title.setCharacterSize(60);
    m_title.setFillColor(sf::Color::White);
    m_title.setPosition({ 300.f, 150.f });

    m_playText.setFont(m_font);
    m_playText.setString("Press ENTER to Play");
    m_playText.setCharacterSize(30);
    m_playText.setFillColor(sf::Color::Green);
    m_playText.setPosition({ 400.f, 350.f });

    m_exitText.setFont(m_font);
    m_exitText.setString("Press ESC to Quit");
    m_exitText.setCharacterSize(30);
    m_exitText.setFillColor(sf::Color::Red);
    m_exitText.setPosition({ 400.f, 400.f });
}

void MenuManager::handleEvent(const sf::Event& event, GameState& state) {
    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>()->code;
        if (key == sf::Keyboard::Key::Enter) {
            state = GameState::Playing;
            ScoreManager::reset();
        }
        else if (key == sf::Keyboard::Key::Escape) {
            state = GameState::GameOver; 
        }
    }
}

void MenuManager::draw(sf::RenderWindow& window) {
    window.draw(m_title);
    window.draw(m_playText);
    window.draw(m_exitText);
}
