#pragma once

#include "GameObject.h"
#include "TransformComponent.h"

namespace engine
{
class VelocityComponent : public Component
{
public:
    explicit VelocityComponent(GameObject* owner, sf::Vector2f velocity = {0.f, 0.f})
        : Component(owner),
          m_velocity(velocity)
    {
    }

    [[nodiscard]] const char* getName() const override
    {
        return "Velocity";
    }

    void onUpdate(float dt) override
    {
        if (auto* transform = getOwner()->getComponent<TransformComponent>())
        {
            transform->move({m_velocity.x * dt, m_velocity.y * dt});
        }
    }

    [[nodiscard]] sf::Vector2f getVelocity() const
    {
        return m_velocity;
    }

    void setVelocity(sf::Vector2f velocity)
    {
        m_velocity = velocity;
    }

private:
    sf::Vector2f m_velocity;
};
} // namespace engine
