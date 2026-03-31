#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "Component.h"

namespace engine
{
class GameObject
{
public:
    explicit GameObject(std::string name = {})
        : m_name(std::move(name))
    {
    }

    template <typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

        auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
        T& componentRef = *component;
        m_components.push_back(std::move(component));

        if (m_started && componentRef.isEnabled())
        {
            componentRef.onStart();
        }

        return componentRef;
    }

    template <typename T>
    [[nodiscard]] T* getComponent() const
    {
        for (const auto& component : m_components)
        {
            if (auto* casted = dynamic_cast<T*>(component.get()))
            {
                return casted;
            }
        }

        return nullptr;
    }

    [[nodiscard]] Component* getComponent(const std::string& componentName) const
    {
        for (const auto& component : m_components)
        {
            if (componentName == component->getName())
            {
                return component.get();
            }
        }

        return nullptr;
    }

    void start()
    {
        if (m_started)
        {
            return;
        }

        for (const auto& component : m_components)
        {
            if (component->isEnabled())
            {
                component->onStart();
            }
        }

        m_started = true;
    }

    void update(float dt)
    {
        if (!m_active)
        {
            return;
        }

        if (!m_started)
        {
            start();
        }

        for (const auto& component : m_components)
        {
            if (component->isEnabled())
            {
                component->onUpdate(dt);
            }
        }
    }

    void render(sf::RenderTarget& target)
    {
        if (!m_active)
        {
            return;
        }

        for (const auto& component : m_components)
        {
            if (component->isEnabled())
            {
                component->onRender(target);
            }
        }
    }

    [[nodiscard]] const std::string& getName() const
    {
        return m_name;
    }

    void setName(std::string name)
    {
        m_name = std::move(name);
    }

    [[nodiscard]] bool isActive() const
    {
        return m_active;
    }

    void setActive(bool active)
    {
        m_active = active;
    }

private:
    std::string m_name;
    std::vector<std::unique_ptr<Component>> m_components;
    bool m_active{true};
    bool m_started{false};
};
} // namespace engine
