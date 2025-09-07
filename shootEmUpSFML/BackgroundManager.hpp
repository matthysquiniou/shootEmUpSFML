#pragma once
#include "SpriteComposite.hpp"
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <cstdint>

class BackgroundManager : public sf::Drawable {
public:
    void addBackground(const std::string& path, int frameCount, float frameDelay, float scrollSpeed, std::uint8_t alpha = 255);

    void update(float dt);

private:
    struct Layer {
        std::shared_ptr<SpriteWrapper> sprite1;
        std::shared_ptr<SpriteWrapper> sprite2;
        std::shared_ptr<Animation> anim;
        float scrollSpeed = 0.f;
        float y1 = 0.f;
        float y2 = -720.f; 
        std::uint8_t alpha = 255;
    };

    std::vector<Layer> m_layers;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
