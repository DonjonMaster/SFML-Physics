#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "GameObject.h"

namespace engine
{
class Scene
{
public:
    GameObject& createObject(const std::string& name = {})
    {
        auto object = std::make_unique<GameObject>(name);
        GameObject& objectRef = *object;
        m_objects.push_back(std::move(object));
        return objectRef;
    }

    void update(float dt)
    {
        for (const auto& object : m_objects)
        {
            object->update(dt);
        }

        removeInactiveObjects();
    }

    void render(sf::RenderTarget& target)
    {
        for (const auto& object : m_objects)
        {
            object->render(target);
        }
    }

    [[nodiscard]] const std::vector<std::unique_ptr<GameObject>>& getObjects() const
    {
        return m_objects;
    }

private:
    void removeInactiveObjects()
    {
        m_objects.erase(
            std::remove_if(m_objects.begin(), m_objects.end(), [](const std::unique_ptr<GameObject>& object) {
                return !object->isActive();
            }),
            m_objects.end());
    }

    std::vector<std::unique_ptr<GameObject>> m_objects;
};
} // namespace engine
