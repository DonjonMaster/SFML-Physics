#pragma once

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "BoxRendererComponent.h"
#include "CircleRendererComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"

namespace engine
{
class OrbitWallsBounceComponent : public Component
{
public:
    explicit OrbitWallsBounceComponent(GameObject* owner,
                                       Scene* scene,
                                       std::vector<std::string> wallNames)
        : Component(owner),
          m_scene(scene),
          m_wallNames(std::move(wallNames))
    {
    }

    [[nodiscard]] const char* getName() const override
    {
        return "OrbitWallsBounce";
    }

    void onUpdate(float dt) override
    {
        (void)dt;

        auto* transform = getOwner()->getComponent<TransformComponent>();
        auto* velocity = getOwner()->getComponent<VelocityComponent>();
        auto* circle = getOwner()->getComponent<CircleRendererComponent>();

        if (!m_scene || !transform || !velocity || !circle)
        {
            return;
        }

        const float radius = circle->getRadius();
        sf::Vector2f center = transform->getPosition() + sf::Vector2f(radius, radius);
        sf::Vector2f vel = velocity->getVelocity();

        for (const std::string& wallName : m_wallNames)
        {
            GameObject* wall = findObjectByName(wallName);
            if (!wall)
            {
                continue;
            }

            auto* wallTransform = wall->getComponent<TransformComponent>();
            auto* wallRenderer = wall->getComponent<BoxRendererComponent>();
            if (!wallTransform || !wallRenderer)
            {
                continue;
            }

            const sf::Vector2f wallCenter = wallTransform->getPosition();
            const sf::Vector2f halfExtents = wallRenderer->getSize() * 0.5f;
            const float angleDeg = wallTransform->getRotation();
            const float angleRad = angleDeg * 3.14159265358979323846f / 180.f;

            const float c = std::cos(angleRad);
            const float s = std::sin(angleRad);

            const sf::Vector2f axisX{c, s};
            const sf::Vector2f axisY{-s, c};

            const sf::Vector2f delta = center - wallCenter;
            const float localX = delta.x * axisX.x + delta.y * axisX.y;
            const float localY = delta.x * axisY.x + delta.y * axisY.y;

            const float clampedX = std::clamp(localX, -halfExtents.x, halfExtents.x);
            const float clampedY = std::clamp(localY, -halfExtents.y, halfExtents.y);

            const sf::Vector2f closest = wallCenter + axisX * clampedX + axisY * clampedY;
            sf::Vector2f diff = center - closest;
            float distSq = diff.x * diff.x + diff.y * diff.y;
            const float radiusSq = radius * radius;

            if (distSq > radiusSq)
            {
                continue;
            }

            float dist = std::sqrt(std::max(distSq, 0.0001f));
            sf::Vector2f normal;

            if (dist < 0.001f)
            {
                if (std::abs(localX) > std::abs(localY))
                {
                    normal = (localX >= 0.f) ? axisX : -axisX;
                }
                else
                {
                    normal = (localY >= 0.f) ? axisY : -axisY;
                }
                dist = 0.001f;
            }
            else
            {
                normal = diff / dist;
            }

            const float penetration = radius - dist;
            center += normal * penetration;

            const float dot = vel.x * normal.x + vel.y * normal.y;
            if (dot < 0.f)
            {
                vel -= 2.f * dot * normal;
            }
        }

        transform->setPosition(center - sf::Vector2f(radius, radius));
        velocity->setVelocity(vel);
    }

private:
    GameObject* findObjectByName(const std::string& name) const
    {
        for (const auto& object : m_scene->getObjects())
        {
            if (object && object->getName() == name)
            {
                return object.get();
            }
        }

        return nullptr;
    }

    Scene* m_scene{};
    std::vector<std::string> m_wallNames;
};
} // namespace engine
