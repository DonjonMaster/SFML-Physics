#pragma once

#include <cmath>
#include <optional>

#include "Component.h"
#include "GameObject.h"
#include "TransformComponent.h"

namespace engine
{
class RotationComponent : public Component
{
public:
    explicit RotationComponent(GameObject* owner,
                               float rotationSpeed,
                               std::optional<sf::Vector2f> pivot = std::nullopt)
        : Component(owner),
          m_rotationSpeed(rotationSpeed),
          m_pivot(pivot)
    {
    }

    [[nodiscard]] const char* getName() const override
    {
        return "Rotation";
    }

    void onStart() override
    {
        if (!m_pivot.has_value())
        {
            return;
        }

        if (auto* transform = getOwner()->getComponent<TransformComponent>())
        {
            m_orbitOffset = transform->getPosition() - *m_pivot;
        }
    }

    void onUpdate(float dt) override
    {
        if (auto* transform = getOwner()->getComponent<TransformComponent>())
        {
            const float delta = m_rotationSpeed * dt;

            if (m_pivot.has_value() && m_orbitOffset.has_value())
            {
                const sf::Vector2f pivot = *m_pivot;
                const sf::Vector2f relative = *m_orbitOffset;
                m_orbitAngle += delta;

                const float radians = m_orbitAngle * 3.14159265358979323846f / 180.f;
                const float c = std::cos(radians);
                const float s = std::sin(radians);

                const sf::Vector2f rotated(
                    relative.x * c - relative.y * s,
                    relative.x * s + relative.y * c);

                transform->setPosition(pivot + rotated);
            }

            transform->setRotation(transform->getRotation() + delta);
        }
    }

private:
    float m_rotationSpeed;
    std::optional<sf::Vector2f> m_pivot;
    std::optional<sf::Vector2f> m_orbitOffset;
    float m_orbitAngle{0.f};
};
} // namespace engine
