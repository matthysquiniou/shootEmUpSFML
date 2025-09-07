#include "ScoreManager.hpp"
#include <iostream>

int ScoreManager::s_score = 0;
sf::Font ScoreManager::s_font;
sf::Text ScoreManager::s_text = sf::Text(s_font);
bool ScoreManager::s_initialized = false;

void ScoreManager::init() {
    if (!s_font.openFromFile("Tuffy.ttf")) {
        std::cerr << "Erreur: impossible de charger assets/fonts/Tuffy.ttf\n";
    }
    s_text.setFont(s_font);
    s_text.setCharacterSize(24);         
    s_text.setFillColor(sf::Color::White);
    s_text.setPosition({ 10.f, 10.f });
    s_initialized = true;
}

void ScoreManager::addScore(int points) {
    if (!s_initialized) init();
    s_score += points;
    s_text.setString("Score: " + std::to_string(s_score));
}

void ScoreManager::reset() {
    if (!s_initialized) init();
    s_score = 0;
    s_text.setString("Score: 0");
}

void ScoreManager::draw(sf::RenderTarget& target) {
    if (!s_initialized) init();
    target.draw(s_text);
}

int ScoreManager::getScore() {
    return s_score;
}