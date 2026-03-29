#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

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

    void pushScene(std::unique_ptr<Scene> scene);

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
        return activeScene();
    }

private:
    void processEvents();
    [[nodiscard]] Scene& activeScene();

    sf::RenderWindow m_window;
    std::vector<std::unique_ptr<Scene>> m_scenes;
    sf::Color m_clearColor;
};
} // namespace engine
