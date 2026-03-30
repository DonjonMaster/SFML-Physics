#pragma once

#include "Component.h"
#include "GameObject.h"
#include <SFML/Graphics.hpp>

namespace engine
{
class CircleWallComponent : public Component
{
public:
    explicit CircleWallComponent(GameObject* owner, float radius, sf::Color color = sf::Color::White, float outlineThickness = 8.f, bool isInternal = false)
        : Component(owner),
          m_circle(radius),
          m_radius(radius),
          m_isInternal(isInternal)
    {
        m_circle.setFillColor(sf::Color::Transparent);
        m_circle.setOutlineColor(color);
        m_circle.setOutlineThickness(outlineThickness);
        m_circle.setOrigin({radius, radius});
    }

    [[nodiscard]] const char* getName() const override
    {
        return "CircleWall";
    }

    void onStart() override
    {
    }

    void onUpdate(float dt) override
    {
        (void)dt;
    }

    void onRender(sf::RenderTarget& target) override
    {
        auto* transform = getOwner()->getComponent<TransformComponent>();
        if (!transform)
        {
            return;
        }

        const sf::Vector2f pos = transform->getPosition();
        m_circle.setPosition(pos);
        
        target.draw(m_circle);
    }

    [[nodiscard]] float getRadius() const
    {
        return m_radius;
    }

    [[nodiscard]] bool isInternal() const
    {
        return m_isInternal;
    }

private:
    sf::CircleShape m_circle;
    float m_radius;
    bool m_isInternal;
};
} // namespace engine
