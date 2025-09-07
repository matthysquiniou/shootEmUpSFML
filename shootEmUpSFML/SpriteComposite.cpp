#include "SpriteComposite.hpp"

// ---------------- Animation ----------------
Animation::Animation(std::vector<Frame> frames, float delaySec)
    : m_frames(std::move(frames)), m_delay(delaySec) {
}

bool Animation::update(float dt) {
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

void Animation::reset() {
    m_index = 0;
    m_elapsed = 0.f;
}

const sf::IntRect& Animation::getRect() const {
    return m_frames[m_index].rect;
}

const sf::IntRect& Animation::getFirstRect() const {
    return m_frames[0].rect;
}

// ---------------- SpriteWrapper ----------------
SpriteWrapper::SpriteWrapper(const std::filesystem::path& path)
    : m_texture(), m_sprite(initSprite(m_texture, path)) {
}

sf::Sprite& SpriteWrapper::get() {
    return m_sprite;
}

const sf::Sprite& SpriteWrapper::get() const {
    return m_sprite;
}

void SpriteWrapper::setRect(const sf::IntRect& rect) {
    m_sprite.setTextureRect(rect);
}

sf::Sprite SpriteWrapper::initSprite(sf::Texture& tex, const std::filesystem::path& path) {
    if (!tex.loadFromFile(path)) {
        throw std::runtime_error("Impossible de charger " + path.string());
    }
    return sf::Sprite(tex);
}

// ---------------- SpriteComposite ----------------
void SpriteComposite::addChild(std::shared_ptr<SpriteWrapper> sprite,
    std::shared_ptr<Animation> anim,
    sf::Vector2f offset) {
    m_children.push_back({ sprite, anim, offset, true, anim != nullptr, false, false });
}

void SpriteComposite::setVisible(std::size_t index, bool visible) {
    if (index < m_children.size()) m_children[index].visible = visible;
}

void SpriteComposite::setAnimationActive(std::size_t index, bool active) {
    if (index < m_children.size()) {
        auto& child = m_children[index];
        if (child.anim) {
            child.stopAfterCurrentLoop = false;
            child.animActive = active;
            if (!active) child.anim->reset();
        }
    }
}

bool SpriteComposite::isAnimationGoing() {
    for (auto& child : m_children) {
        if (child.animActive) return true;
    }
    return false;
}

void SpriteComposite::update(float dt) {
    for (auto& child : m_children) {
        if (!child.visible) continue;
        if (child.anim) {
            bool finishedLoop = false;
            if (child.animActive) finishedLoop = child.anim->update(dt);
            child.sprite->setRect(child.anim->getRect());

            if (child.stopAfterCurrentLoop && finishedLoop) {
                child.animActive = false;
                child.stopAfterCurrentLoop = false;
                if (child.stopAfterCurrentLoopVisibleToggle) {
                    child.visible = false;
                    child.stopAfterCurrentLoopVisibleToggle = false;
                }
            }
        }
    }
}

void SpriteComposite::stopAnimationAfterLoop(std::size_t index, bool visibleToggle) {
    if (index < m_children.size()) {
        auto& child = m_children[index];
        if (child.anim) {
            child.stopAfterCurrentLoop = true;
            child.stopAfterCurrentLoopVisibleToggle = visibleToggle;
        }
    }
}

void SpriteComposite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
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

size_t SpriteComposite::getChildrenCount() {
    return m_children.size();
}

SpriteComposite::Child SpriteComposite::getChild(int index) {
    return m_children[index];
}
