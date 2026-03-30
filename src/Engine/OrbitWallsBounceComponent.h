#pragma once

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "BoxRendererComponent.h"
#include "CircleRendererComponent.h"
#include "CircleWallComponent.h"
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
            if (!wallTransform)
            {
                continue;
            }

            // Check if it's a circle wall
            auto* circleWall = wall->getComponent<CircleWallComponent>();
            if (circleWall)
            {
                const sf::Vector2f wallCenter = wallTransform->getPosition();
                const float wallRadius = circleWall->getRadius();
                
                sf::Vector2f delta = center - wallCenter;
                float distSq = delta.x * delta.x + delta.y * delta.y;
                float dist = std::sqrt(std::max(distSq, 0.0001f));
                
                sf::Vector2f normal;
                constexpr float collisionMargin = 2.f;
                
                if (circleWall->isInternal())
                {
                    // Internal circle: keep the ball inside the hollow area.
                    // Allowed center distance from the wall center is <= maxDist.
                    const float maxDist = wallRadius - radius - collisionMargin;
                    if (dist <= maxDist)
                    {
                        continue;
                    }

                    if (dist < 0.001f)
                    {
                        normal = sf::Vector2f(1.f, 0.f);
                        dist = 0.001f;
                    }
                    else
                    {
                        // Inward normal (from current point toward circle center)
                        normal = -delta / dist;
                    }

                    const float penetration = dist - maxDist;
                    if (penetration > 0.f)
                    {
                        center += normal * (penetration + 1.f);
                    }

                    const float dot = vel.x * normal.x + vel.y * normal.y;
                    if (dot < 0.f)
                    {
                        vel -= 2.f * dot * normal;
                    }
                }
                else
                {
                    // External circle: ball bounces from OUTSIDE (original behavior)
                    const float combinedRadius = radius + wallRadius + collisionMargin;
                    const float combinedRadiusSq = combinedRadius * combinedRadius;
                    
                    if (distSq > combinedRadiusSq)
                    {
                        continue;
                    }
                    
                    if (dist < 0.001f)
                    {
                        normal = sf::Vector2f(1.f, 0.f);
                        dist = 0.001f;
                    }
                    else
                    {
                        normal = delta / dist;
                    }
                    
                    const float penetration = combinedRadius - dist;
                    if (penetration > 0.f)
                    {
                        center += normal * (penetration + 1.f);
                    }
                    
                    const float dot = vel.x * normal.x + vel.y * normal.y;
                    if (dot < 0.f)
                    {
                        vel -= 2.f * dot * normal;
                    }
                }
            }
            else
            {
                // Rectangle wall collision
                auto* wallRenderer = wall->getComponent<BoxRendererComponent>();
                if (!wallRenderer)
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

                // Determine which zone the ball hits: left end, middle, or right end
                bool isLeftEnd = (localX < -halfExtents.x);
                bool isRightEnd = (localX > halfExtents.x);
                bool isMiddle = (!isLeftEnd && !isRightEnd);

                const float clampedX = std::clamp(localX, -halfExtents.x, halfExtents.x);
                const float clampedY = std::clamp(localY, -halfExtents.y, halfExtents.y);

                const sf::Vector2f closest = wallCenter + axisX * clampedX + axisY * clampedY;
                sf::Vector2f diff = center - closest;
                float distSq = diff.x * diff.x + diff.y * diff.y;
                
                constexpr float collisionMargin = 2.f;
                const float effectiveRadius = radius + collisionMargin;
                const float radiusSq = effectiveRadius * effectiveRadius;

                if (distSq > radiusSq)
                {
                    continue;
                }

                float dist = std::sqrt(std::max(distSq, 0.0001f));
                sf::Vector2f normal;

                if (dist < 0.001f)
                {
                    if (isMiddle)
                    {
                        // Middle: bounce perpendicular (normal behavior)
                        normal = (localY >= 0.f) ? axisY : -axisY;
                    }
                    else
                    {
                        // Ends: bounce parallel (opposite behavior)
                        normal = (localX >= 0.f) ? axisX : -axisX;
                    }
                    dist = 0.001f;
                }
                else
                {
                    // Use the actual direction from closest point
                    normal = diff / dist;
                }

                const float penetration = effectiveRadius - dist;
                if (penetration > 0.f)
                {
                    // Push ball out of collision with extra buffer
                    center += normal * (penetration + 1.f);
                }

                const float dot = vel.x * normal.x + vel.y * normal.y;
                if (dot < 0.f)
                {
                    vel -= 2.f * dot * normal;
                }
            }
        }

        // Ball-ball collisions (equal mass, near-elastic response)
        for (const auto& object : m_scene->getObjects())
        {
            if (!object)
            {
                continue;
            }

            GameObject* other = object.get();
            if (other == getOwner())
            {
                continue;
            }

            // Avoid double-solving the same pair in the same frame
            if (getOwner()->getName() >= other->getName())
            {
                continue;
            }

            auto* otherTransform = other->getComponent<TransformComponent>();
            auto* otherVelocity = other->getComponent<VelocityComponent>();
            auto* otherCircle = other->getComponent<CircleRendererComponent>();
            if (!otherTransform || !otherVelocity || !otherCircle)
            {
                continue;
            }

            const float otherRadius = otherCircle->getRadius();
            sf::Vector2f otherCenter = otherTransform->getPosition() + sf::Vector2f(otherRadius, otherRadius);
            sf::Vector2f otherVel = otherVelocity->getVelocity();

            sf::Vector2f delta = otherCenter - center;
            float distSq = delta.x * delta.x + delta.y * delta.y;
            const float sumRadius = radius + otherRadius;
            const float sumRadiusSq = sumRadius * sumRadius;

            if (distSq >= sumRadiusSq)
            {
                continue;
            }

            float dist = std::sqrt(std::max(distSq, 0.0001f));
            sf::Vector2f normal;
            if (dist < 0.001f)
            {
                normal = sf::Vector2f(1.f, 0.f);
                dist = 0.001f;
            }
            else
            {
                normal = delta / dist;
            }

            // Positional correction to remove overlap
            const float penetration = sumRadius - dist;
            if (penetration > 0.f)
            {
                const float correction = (penetration * 0.5f) + 0.2f;
                center -= normal * correction;
                otherCenter += normal * correction;
                otherTransform->setPosition(otherCenter - sf::Vector2f(otherRadius, otherRadius));
            }

            // Elastic impulse along collision normal (equal masses)
            const sf::Vector2f relativeVel = otherVel - vel;
            const float relAlongNormal = relativeVel.x * normal.x + relativeVel.y * normal.y;
            if (relAlongNormal >= 0.f)
            {
                continue;
            }

            constexpr float restitution = 0.98f;
            const float impulse = -(1.f + restitution) * relAlongNormal * 0.5f;

            vel -= normal * impulse;
            otherVel += normal * impulse;
            otherVelocity->setVelocity(otherVel);
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
