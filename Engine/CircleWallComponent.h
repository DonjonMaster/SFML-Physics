#pragma once

#include <cmath>

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

        if (!m_hasOpenings)
        {
            target.draw(m_circle);
            return;
        }

        // Draw the ring manually to keep visible gaps at entry/exit angles.
        const sf::Color outlineColor = m_circle.getOutlineColor();
        const float outlineThickness = std::max(1.f, std::abs(m_circle.getOutlineThickness()));
        constexpr int samples = 720;
        const float pi = 3.14159265358979323846f;

        const int halfLayers = static_cast<int>(outlineThickness * 0.5f);
        for (int layer = -halfLayers; layer <= halfLayers; ++layer)
        {
            const float drawRadius = std::max(1.f, m_radius + static_cast<float>(layer));
            sf::VertexArray strip(sf::PrimitiveType::LineStrip);

            for (int i = 0; i <= samples; ++i)
            {
                const float angleDeg = (static_cast<float>(i) / static_cast<float>(samples)) * 360.f;
                const bool inOpening = isAngleInOpening(angleDeg);

                if (inOpening)
                {
                    if (strip.getVertexCount() >= 2)
                    {
                        target.draw(strip);
                    }
                    strip.clear();
                    continue;
                }

                const float angleRad = angleDeg * pi / 180.f;
                const sf::Vector2f p{
                    pos.x + drawRadius * std::cos(angleRad),
                    pos.y + drawRadius * std::sin(angleRad)
                };

                strip.append(sf::Vertex{p, outlineColor});
            }

            if (strip.getVertexCount() >= 2)
            {
                target.draw(strip);
            }
        }
    }

    [[nodiscard]] float getRadius() const
    {
        return m_radius;
    }

    [[nodiscard]] bool isInternal() const
    {
        return m_isInternal;
    }

    void setOpenings(float entryAngleDeg, float exitAngleDeg, float openingHalfAngleDeg)
    {
        m_hasOpenings = true;
        m_entryAngleDeg = normalizeAngle(entryAngleDeg);
        m_exitAngleDeg = normalizeAngle(exitAngleDeg);
        m_openingHalfAngleDeg = std::max(0.f, openingHalfAngleDeg);
    }

    [[nodiscard]] bool isAngleInOpening(float angleDeg) const
    {
        if (!m_hasOpenings)
        {
            return false;
        }

        const float angle = normalizeAngle(angleDeg);
        return angleDistanceDeg(angle, m_entryAngleDeg) <= m_openingHalfAngleDeg
            || angleDistanceDeg(angle, m_exitAngleDeg) <= m_openingHalfAngleDeg;
    }

private:
    static float normalizeAngle(float angleDeg)
    {
        float a = std::fmod(angleDeg, 360.f);
        if (a < 0.f)
        {
            a += 360.f;
        }
        return a;
    }

    static float angleDistanceDeg(float a, float b)
    {
        float d = std::abs(a - b);
        return (d > 180.f) ? 360.f - d : d;
    }

    sf::CircleShape m_circle;
    float m_radius;
    bool m_isInternal;
    bool m_hasOpenings{false};
    float m_entryAngleDeg{0.f};
    float m_exitAngleDeg{180.f};
    float m_openingHalfAngleDeg{0.f};
};
} // namespace engine
