#pragma once

#include "GameObject.h"
#include "TransformComponent.h"

namespace engine
{
class BoxRendererComponent : public Component
{
public:
    explicit BoxRendererComponent(GameObject* owner,
                                  sf::Vector2f size,
                                  sf::Color color = sf::Color::White,
                                  sf::Vector2f origin = {0.f, 0.f})
        : Component(owner)
    {
        m_shape.setSize(size);
        m_shape.setFillColor(color);
        m_shape.setOrigin(origin);
    }

    [[nodiscard]] const char* getName() const override
    {
        return "BoxRenderer";
    }

    void setFillColor(sf::Color color)
    {
        m_shape.setFillColor(color);
    }

    [[nodiscard]] sf::Vector2f getSize() const
    {
        return m_shape.getSize();
    }

    void setSize(sf::Vector2f size)
    {
        m_shape.setSize(size);
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
    sf::RectangleShape m_shape;
};
} // namespace engine
