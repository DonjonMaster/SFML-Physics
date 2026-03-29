#pragma once

#include "Component.h"

namespace engine
{
class TransformComponent : public Component
{
public:
    explicit TransformComponent(GameObject* owner,
                                sf::Vector2f position = {0.f, 0.f},
                                float rotation = 0.f,
                                sf::Vector2f scale = {1.f, 1.f})
        : Component(owner),
          m_position(position),
          m_rotation(rotation),
          m_scale(scale)
    {
    }

    [[nodiscard]] const char* getName() const override
    {
        return "Transform";
    }

    [[nodiscard]] sf::Vector2f getPosition() const
    {
        return m_position;
    }

    void setPosition(sf::Vector2f position)
    {
        m_position = position;
    }

    void move(sf::Vector2f offset)
    {
        m_position += offset;
    }

    [[nodiscard]] float getRotation() const
    {
        return m_rotation;
    }

    void setRotation(float rotation)
    {
        m_rotation = rotation;
    }

    [[nodiscard]] sf::Vector2f getScale() const
    {
        return m_scale;
    }

    void setScale(sf::Vector2f scale)
    {
        m_scale = scale;
    }

private:
    sf::Vector2f m_position;
    float m_rotation;
    sf::Vector2f m_scale;
};
} // namespace engine
