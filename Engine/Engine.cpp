#include "Engine.h"

#include <optional>

namespace engine
{
Engine::Engine(const EngineConfig& config)
    : m_window(sf::VideoMode(config.windowSize), config.title),
      m_clearColor(config.clearColor)
{
    m_window.setFramerateLimit(config.framerateLimit);
    m_scenes.push_back(std::make_unique<Scene>());
}

void Engine::pushScene(std::unique_ptr<Scene> scene)
{
    if (!scene)
    {
        return;
    }

    m_scenes.push_back(std::move(scene));
}

void Engine::run(const std::function<void(float, Scene&)>& userUpdate)
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        processEvents();

        const float dt = clock.restart().asSeconds();
        Scene& scene = activeScene();
        scene.update(dt);
        CollisionSystem::process(scene);

        if (userUpdate)
        {
            userUpdate(dt, scene);
        }

        m_window.clear(m_clearColor);
        scene.render(m_window);
        m_window.display();
    }
}

Scene& Engine::activeScene()
{
    if (m_scenes.empty())
    {
        m_scenes.push_back(std::make_unique<Scene>());
    }

    return *m_scenes.back();
}

void Engine::processEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }
    }
}
} // namespace engine
