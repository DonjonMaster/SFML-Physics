#pragma once

#include <functional>

#include "Component.h"

namespace engine
{
class ColliderComponent : public Component
{
public:
    explicit ColliderComponent(GameObject* owner,
                               sf::Vector2f size,
                               sf::Vector2f offset = {0.f, 0.f},
                               bool isTrigger = false)
        : Component(owner),
          m_size(size),
          m_offset(offset),
          m_isTrigger(isTrigger)
    {
    }

    [[nodiscard]] const char* getName() const override
    {
        return "Collider";
    }

    [[nodiscard]] sf::FloatRect getWorldBounds() const;

    [[nodiscard]] bool intersects(const ColliderComponent& other) const
    {
        return getWorldBounds().findIntersection(other.getWorldBounds()).has_value();
    }

    [[nodiscard]] sf::Vector2f getSize() const
    {
        return m_size;
    }

    void setSize(sf::Vector2f size)
    {
        m_size = size;
    }

    [[nodiscard]] sf::Vector2f getOffset() const
    {
        return m_offset;
    }

    void setOffset(sf::Vector2f offset)
    {
        m_offset = offset;
    }

    [[nodiscard]] bool isTrigger() const
    {
        return m_isTrigger;
    }

    void setIsTrigger(bool isTrigger)
    {
        m_isTrigger = isTrigger;
    }

    void setDebugDraw(bool debugDraw, sf::Color color = sf::Color::Green)
    {
        m_debugDraw = debugDraw;
        m_debugColor = color;
    }

    void setOnCollision(const std::function<void(GameObject&)>& callback)
    {
        m_onCollision = callback;
    }

    void notifyCollision(GameObject& other)
    {
        if (m_onCollision)
        {
            m_onCollision(other);
        }
    }

    void onRender(sf::RenderTarget& target) override;

private:
    sf::Vector2f m_size;
    sf::Vector2f m_offset;
    bool m_isTrigger{false};
    bool m_debugDraw{false};
    sf::Color m_debugColor{sf::Color::Green};
    std::function<void(GameObject&)> m_onCollision;
};
} // namespace engine
