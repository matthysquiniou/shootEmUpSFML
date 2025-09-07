#include "BackgroundManager.hpp"

void BackgroundManager::addBackground(const std::string& path, int frameCount, float frameDelay, float scrollSpeed, std::uint8_t alpha) {
    auto sprite1 = std::make_shared<SpriteWrapper>(path);
    auto sprite2 = std::make_shared<SpriteWrapper>(path);

    std::vector<Frame> frames;
    for (int i = 0; i < frameCount; ++i) {
        frames.push_back(Frame{ sf::IntRect({i * 1280, 0}, {1280, 720}) });
    }

    auto anim = std::make_shared<Animation>(frames, frameDelay);

    sprite1->setRect(anim->getRect());
    sprite2->setRect(anim->getRect());

    sprite1->get().setColor(sf::Color(255,255,255,alpha));
    sprite2->get().setColor(sf::Color(255,255,255,alpha));

    m_layers.push_back({ sprite1, sprite2, anim, scrollSpeed, 0.f, -720.f, alpha });
}

void BackgroundManager::update(float dt) {
    for (auto& layer : m_layers) {
        layer.anim->update(dt);
        layer.sprite1->setRect(layer.anim->getRect());
        layer.sprite2->setRect(layer.anim->getRect());

        layer.y1 += layer.scrollSpeed * dt;
        layer.y2 += layer.scrollSpeed * dt;

        if (layer.y1 >= 720.f) layer.y1 = layer.y2 - 720.f;
        if (layer.y2 >= 720.f) layer.y2 = layer.y1 - 720.f;
    }
}

void BackgroundManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& layer : m_layers) {
        auto spr1 = layer.sprite1->get();
        spr1.setPosition({ 0.f, layer.y1 });
        target.draw(spr1, states);

        auto spr2 = layer.sprite2->get();
        spr2.setPosition({ 0.f, layer.y2 });
        target.draw(spr2, states);
    }
}
