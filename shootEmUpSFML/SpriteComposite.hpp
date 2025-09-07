#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <vector>
#include <memory>
#include <stdexcept>

struct Frame {
    sf::IntRect rect;
};

class Animation {
public:
    Animation(std::vector<Frame> frames, float delaySec);

    bool update(float dt);
    void reset();

    const sf::IntRect& getRect() const;
    const sf::IntRect& getFirstRect() const;

private:
    std::vector<Frame> m_frames;
    float m_delay;
    float m_elapsed = 0.f;
    std::size_t m_index = 0;
};

class SpriteWrapper {
public:
    explicit SpriteWrapper(const std::filesystem::path& path);

    sf::Sprite& get();
    const sf::Sprite& get() const;

    void setRect(const sf::IntRect& rect);

private:
    static sf::Sprite initSprite(sf::Texture& tex, const std::filesystem::path& path);

    sf::Texture m_texture;
    sf::Sprite  m_sprite;
};

class SpriteComposite : public sf::Drawable, public sf::Transformable {
public:
    bool flipX = false;
    bool flipY = false;

    struct Child {
        std::shared_ptr<SpriteWrapper> sprite;
        std::shared_ptr<Animation> anim;
        sf::Vector2f offset{ 0.f,0.f };
        bool visible = true;
        bool animActive = true;
        bool stopAfterCurrentLoop = false;
        bool stopAfterCurrentLoopVisibleToggle = false;
    };

    void addChild(std::shared_ptr<SpriteWrapper> sprite,
        std::shared_ptr<Animation> anim = nullptr,
        sf::Vector2f offset = { 0.f,0.f });

    void setVisible(std::size_t index, bool visible);
    void setAnimationActive(std::size_t index, bool active);
    bool isAnimationGoing();
    void update(float dt);
    void stopAnimationAfterLoop(std::size_t index, bool visibleToggle = false);
    size_t getChildrenCount();
    Child getChild(int index);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    std::vector<Child> m_children;
};
