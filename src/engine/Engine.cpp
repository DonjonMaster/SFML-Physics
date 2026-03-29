#include "Engine.h"

#include <optional>

namespace engine
{
Engine::Engine(const EngineConfig& config)
    : m_window(sf::VideoMode(config.windowSize), config.title),
      m_clearColor(config.clearColor)
{
    m_window.setFramerateLimit(config.framerateLimit);
}

void Engine::run(const std::function<void(float, Scene&)>& userUpdate)
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        processEvents();

        const float dt = clock.restart().asSeconds();
        m_scene.update(dt);
        CollisionSystem::process(m_scene);

        if (userUpdate)
        {
            userUpdate(dt, m_scene);
        }

        m_window.clear(m_clearColor);
        m_scene.render(m_window);
        m_window.display();
    }
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
