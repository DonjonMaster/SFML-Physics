#pragma once

#include "GameObject.h"
#include "TransformComponent.h"

namespace engine
{
class CircleRendererComponent : public Component
{
public:
    explicit CircleRendererComponent(GameObject* owner,
                                     float radius,
                                     sf::Color color = sf::Color::White,
                                     sf::Vector2f origin = {0.f, 0.f})
        : Component(owner)
    {
        m_shape.setRadius(radius);
        m_shape.setFillColor(color);
        m_shape.setOrigin(origin);
    }

    [[nodiscard]] const char* getName() const override
    {
        return "CircleRenderer";
    }

    void setFillColor(sf::Color color)
    {
        m_shape.setFillColor(color);
    }

    [[nodiscard]] float getRadius() const
    {
        return m_shape.getRadius();
    }

    void setRadius(float radius)
    {
        m_shape.setRadius(radius);
    }

    void onRender(sf::RenderTarget& target) override
    {
        if (auto* transform = getOwner()->getComponent<TransformComponent>())
        {
            m_shape.setPosition(transform->getPosition());
            m_shape.setScale(transform->getScale());
            m_shape.setRotation(sf::degrees(transform->getRotation()));
        }

        target.draw(m_shape);
    }

private:
    sf::CircleShape m_shape;
};
} // namespace engine
