#pragma once

#include <SFML/Graphics.hpp>

namespace engine
{
class GameObject;

class Component
{
public:
    explicit Component(GameObject* owner)
        : m_owner(owner)
    {
    }

    virtual ~Component() = default;

    virtual const char* getName() const = 0;
    virtual void onStart() {}
    virtual void onUpdate(float dt)
    {
        (void)dt;
    }
    virtual void onRender(sf::RenderTarget& target)
    {
        (void)target;
    }

    [[nodiscard]] bool isEnabled() const
    {
        return m_enabled;
    }

    void setEnabled(bool enabled)
    {
        m_enabled = enabled;
    }

protected:
    [[nodiscard]] GameObject* getOwner() const
    {
        return m_owner;
    }

private:
    GameObject* m_owner{};
    bool m_enabled{true};
};
} // namespace engine
