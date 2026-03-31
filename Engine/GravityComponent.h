#pragma once

#include "GameObject.h"
#include "VelocityComponent.h"

namespace engine
{
class GravityComponent : public Component
{
public:
    explicit GravityComponent(GameObject* owner, float gravity = 600.f)
        : Component(owner),
          m_gravity(gravity)
    {
    }

    [[nodiscard]] const char* getName() const override
    {
        return "Gravity";
    }

    void onUpdate(float dt) override
    {
        if (auto* velocity = getOwner()->getComponent<VelocityComponent>())
        {
            sf::Vector2f v = velocity->getVelocity();
            v.y += m_gravity * dt;
            velocity->setVelocity(v);
        }
    }

    [[nodiscard]] float getGravity() const
    {
        return m_gravity;
    }

    void setGravity(float gravity)
    {
        m_gravity = gravity;
    }

private:
    float m_gravity;
};
} // namespace engine
