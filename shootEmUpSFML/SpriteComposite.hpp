#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <vector>
#include <memory>

struct Frame {
    sf::IntRect rect;
};

class Animation {
public:
    Animation(std::vector<Frame> frames, float delaySec)
        : m_frames(std::move(frames)), m_delay(delaySec) {
    }

    bool update(float dt) {
        if (m_frames.empty()) return false;
        m_elapsed += dt;
        bool finished = false;
        if (m_elapsed >= m_delay) {
            m_elapsed = 0.f;
            m_index++;
            if (m_index >= m_frames.size()) {
                m_index = 0;
                finished = true;
            }
        }
        return finished;
    }

    void reset() { m_index = 0; m_elapsed = 0.f; }

    const sf::IntRect& getRect() const { return m_frames[m_index].rect; }

    const sf::IntRect& getFirstRect() const { return m_frames[0].rect; } 

private:
    std::vector<Frame> m_frames;
    float m_delay;
    float m_elapsed = 0.f;
    std::size_t m_index = 0;
};



class SpriteWrapper {
public:
    explicit SpriteWrapper(const std::filesystem::path& path)
        : m_texture()                        
        , m_sprite(initSprite(m_texture, path))
    {
    }

    sf::Sprite& get() { return m_sprite; }
    const sf::Sprite& get() const { return m_sprite; }

    void setRect(const sf::IntRect& rect) { m_sprite.setTextureRect(rect); }

private:
    static sf::Sprite initSprite(sf::Texture& tex, const std::filesystem::path& path) {
        if (!tex.loadFromFile(path)) {
            throw std::runtime_error("Impossible de charger " + path.string());
        }
        return sf::Sprite(tex); 
    }

    sf::Texture m_texture; 
    sf::Sprite  m_sprite;
};

struct ChildSprite {
    SpriteWrapper& sprite;
    sf::Vector2f offset;
};

class SpriteComposite : public sf::Drawable, public sf::Transformable {
public:
    bool flipX = false;
    bool flipY = false;
    struct Child {
        std::shared_ptr<SpriteWrapper> sprite;
        std::shared_ptr<Animation> anim;
        sf::Vector2f offset{ 0.f, 0.f };
        bool visible = true;             
        bool animActive = true;        
        bool stopAfterCurrentLoop = false;
        bool stopAfterCurrentLoopVisibleToggle = false;
    };

    void addChild(std::shared_ptr<SpriteWrapper> sprite,
        std::shared_ptr<Animation> anim = nullptr,
        sf::Vector2f offset = { 0.f, 0.f })
    {
        m_children.push_back({ sprite, anim, offset, true, anim != nullptr, false, false });
    }

    void setVisible(std::size_t index, bool visible) {
        if (index < m_children.size()) m_children[index].visible = visible;
    }

    void setAnimationActive(std::size_t index, bool active) {
        if (index < m_children.size()) {
            auto& child = m_children[index];
            if (child.anim) {
                child.stopAfterCurrentLoop = false;
                child.animActive = active;
                if (!active) child.anim->reset();
            }
        }
    }

    bool isAnimationGoing() {
        for (auto& child : m_children) {
            if (child.animActive == true)
            {
                return true;
            }
        }
        return false;
    }

    void update(float dt) {
        for (auto& child : m_children) {
            if (!child.visible) continue;

            if (child.anim) {
                bool finishedLoop = false;
                if (child.animActive) {
                    finishedLoop = child.anim->update(dt);
                }

                child.sprite->setRect(child.anim->getRect());

                if (child.stopAfterCurrentLoop && finishedLoop) {
                    child.animActive = false;
                    child.stopAfterCurrentLoop = false;
                    if (child.stopAfterCurrentLoopVisibleToggle)
                    {
                        child.visible = false;
                        child.stopAfterCurrentLoopVisibleToggle = false;
                    }
                }
            }
        }
    }

    void stopAnimationAfterLoop(std::size_t index, bool visibleToggle = false) {
        if (index < m_children.size()) {
            auto& child = m_children[index];
            if (child.anim) {
                child.stopAfterCurrentLoop = true;
                child.stopAfterCurrentLoopVisibleToggle = visibleToggle;
            }
        }
    }

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        for (auto& child : m_children) {
            if (!child.visible) continue;

            sf::Sprite sprite = child.sprite->get();  
            sprite.setPosition(child.offset);

            sf::FloatRect bounds = sprite.getLocalBounds();
            if (flipX || flipY) {
                sprite.setOrigin({ flipX ? bounds.size.x : 0.f,
                    flipY ? bounds.size.y : 0.f });
                sprite.setScale({ flipX ? -1.f : 1.f,
                    flipY ? -1.f : 1.f });
            }

            target.draw(sprite, states);
        }
    }

    std::vector<Child> m_children;
};