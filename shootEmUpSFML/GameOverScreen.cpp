#include "GameOverScreen.hpp"
#include "ScoreManager.hpp"
#include <iostream>

GameOverScreen::GameOverScreen() {
    if (!m_font.openFromFile("Tuffy.ttf")) {
        std::cerr << "Erreur: impossible de charger Tuffy.ttf\n";
    }

    m_gameOverText.setFont(m_font);
    m_gameOverText.setString("GAME OVER");
    m_gameOverText.setCharacterSize(60);
    m_gameOverText.setFillColor(sf::Color::Red);
    m_gameOverText.setPosition({ 400.f, 200.f });

    m_retryText.setFont(m_font);
    m_retryText.setString("Press ENTER to Retry");
    m_retryText.setCharacterSize(30);
    m_retryText.setFillColor(sf::Color::White);
    m_retryText.setPosition({ 400.f, 350.f });

    m_exitText.setFont(m_font);
    m_exitText.setString("Press ESC to Quit");
    m_exitText.setCharacterSize(30);
    m_exitText.setFillColor(sf::Color::White);
    m_exitText.setPosition({ 400.f, 400.f });
}

void GameOverScreen::handleEvent(const sf::Event& event, GameState& state) {
    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>()->code;
        if (key == sf::Keyboard::Key::Enter) {
            state = GameState::Playing;
            ScoreManager::reset(); 
        }
        else if (key == sf::Keyboard::Key::Escape) {
            state = GameState::Menu;
        }
    }
}

void GameOverScreen::draw(sf::RenderWindow& window, int finalScore) {
    window.draw(m_gameOverText);

    sf::Text scoreText = sf::Text(m_font);
    scoreText.setString("Final Score: " + std::to_string(finalScore));
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition({ 400.f, 280.f });

    window.draw(scoreText);
    window.draw(m_retryText);
    window.draw(m_exitText);
}
