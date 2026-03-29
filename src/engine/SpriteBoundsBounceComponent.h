#pragma once

#include "CircleRendererComponent.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"

namespace engine
{
class SpriteBoundsBounceComponent : public Component
{
public:
    explicit SpriteBoundsBounceComponent(GameObject* owner,
                                         sf::FloatRect playArea)
        : Component(owner),
          m_playArea(playArea)
    {
    }

    [[nodiscard]] const char* getName() const override
    {
        return "SpriteBoundsBounce";
    }

    void onUpdate(float dt) override
    {
        (void)dt;

        auto* transform = getOwner()->getComponent<TransformComponent>();
        auto* velocity = getOwner()->getComponent<VelocityComponent>();
        auto* circle = getOwner()->getComponent<CircleRendererComponent>();

        if (!transform || !velocity || !circle)
        {
            return;
        }

        sf::Vector2f pos = transform->getPosition();
        sf::Vector2f vel = velocity->getVelocity();
        const float diameter = circle->getRadius() * 2.f;

        if (pos.x < m_playArea.position.x)
        {
            pos.x = m_playArea.position.x;
            if (vel.x < 0.f)
            {
                vel.x = -vel.x;
            }
        }
        else if (pos.x + diameter > m_playArea.position.x + m_playArea.size.x)
        {
            pos.x = m_playArea.position.x + m_playArea.size.x - diameter;
            if (vel.x > 0.f)
            {
                vel.x = -vel.x;
            }
        }

        if (pos.y < m_playArea.position.y)
        {
            pos.y = m_playArea.position.y;
            if (vel.y < 0.f)
            {
                vel.y = -vel.y;
            }
        }
        else if (pos.y + diameter > m_playArea.position.y + m_playArea.size.y)
        {
            pos.y = m_playArea.position.y + m_playArea.size.y - diameter;
            if (vel.y > 0.f)
            {
                vel.y = -vel.y;
            }
        }

        transform->setPosition(pos);
        velocity->setVelocity(vel);
    }

private:
    sf::FloatRect m_playArea;
};
} // namespace engine
