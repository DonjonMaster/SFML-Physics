#pragma once

#include <vector>

#include "ColliderComponent.h"
#include "Scene.h"

namespace engine
{
class CollisionSystem
{
public:
    static void process(Scene& scene)
    {
        struct ColliderEntry
        {
            GameObject* object;
            ColliderComponent* collider;
        };

        std::vector<ColliderEntry> colliders;

        for (const auto& object : scene.getObjects())
        {
            if (!object->isActive())
            {
                continue;
            }

            if (auto* collider = object->getComponent<ColliderComponent>())
            {
                colliders.push_back({object.get(), collider});
            }
        }

        for (std::size_t i = 0; i < colliders.size(); ++i)
        {
            for (std::size_t j = i + 1; j < colliders.size(); ++j)
            {
                ColliderEntry& first = colliders[i];
                ColliderEntry& second = colliders[j];

                if (!first.collider->intersects(*second.collider))
                {
                    continue;
                }

                first.collider->notifyCollision(*second.object);
                second.collider->notifyCollision(*first.object);
            }
        }
    }
};
} // namespace engine
