#pragma once

#include <functional>
#include <string>

#include <SFML/Graphics.hpp>

#include "CollisionSystem.h"
#include "Scene.h"

namespace engine
{
struct EngineConfig
{
    sf::Vector2u windowSize{900u, 600u};
    std::string title{"Generic SFML Engine"};
    unsigned int framerateLimit{60};
    sf::Color clearColor{sf::Color(16, 18, 22)};
};

class Engine
{
public:
    explicit Engine(const EngineConfig& config = {});

    void run(const std::function<void(float, Scene&)>& userUpdate = {});

    [[nodiscard]] bool isOpen() const
    {
        return m_window.isOpen();
    }

    [[nodiscard]] sf::RenderWindow& getWindow()
    {
        return m_window;
    }

    [[nodiscard]] Scene& getScene()
    {
        return m_scene;
    }

private:
    void processEvents();

    sf::RenderWindow m_window;
    Scene m_scene;
    sf::Color m_clearColor;
};
} // namespace engine
