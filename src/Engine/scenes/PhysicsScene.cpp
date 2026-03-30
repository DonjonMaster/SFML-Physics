#include "PhysicsScene.h"

#include <string>

#include "../CircleRendererComponent.h"
#include "../CircleWallComponent.h"
#include "../GameObject.h"
#include "../OrbitWallsBounceComponent.h"
#include "../TransformComponent.h"
#include "../VelocityComponent.h"

namespace engine::scenes
{
std::unique_ptr<Scene> createPhysicsScene()
{
    constexpr float windowWidth = 1920.f;
    constexpr float windowHeight = 1080.f;

    // Create scene
    auto scene = std::make_unique<Scene>();

    // Create one internal circular wall (hollow circle where balls bounce inside)
    auto pushInternalCircleWall = [&](const std::string& name, sf::Vector2f position, float radius, sf::Color color)
    {
        auto wall = std::make_unique<GameObject>(name);
        wall->addComponent<TransformComponent>(position);
        wall->addComponent<CircleWallComponent>(radius, color, 8.f, true);  // Internal
        scene->pushObject(std::move(wall));
    };

    // Single large hollow-circle level
    const float centerX = windowWidth * 0.5f;
    const float centerY = windowHeight * 0.5f;
    const float levelRadius = 430.f;
    pushInternalCircleWall("LevelCircle", {centerX, centerY}, levelRadius, sf::Color(100, 150, 220));

    // Create moving objects
    auto pushMover = [&](const std::string& name, sf::Vector2f position, float radius, sf::Vector2f velocity, sf::Color color)
    {
        auto mover = std::make_unique<GameObject>(name);
        mover->addComponent<TransformComponent>(position);
        mover->addComponent<CircleRendererComponent>(radius, color);
        mover->addComponent<VelocityComponent>(velocity);
        
        std::vector<std::string> walls{"LevelCircle"};
        
        mover->addComponent<OrbitWallsBounceComponent>(scene.get(), walls);
        scene->pushObject(std::move(mover));
    };

    // Movers positioned inside the single circle
    pushMover("MoverA", {centerX - 180.f, centerY - 90.f}, 25.f, {220.f, -170.f}, sf::Color(80, 180, 255));
    pushMover("MoverB", {centerX + 180.f, centerY - 90.f}, 25.f, {-220.f, -170.f}, sf::Color(255, 120, 120));
    pushMover("MoverC", {centerX - 120.f, centerY + 120.f}, 25.f, {180.f, 210.f}, sf::Color(120, 255, 120));
    pushMover("MoverD", {centerX + 120.f, centerY + 120.f}, 25.f, {-180.f, 210.f}, sf::Color(255, 255, 120));

    return scene;
}
} // namespace engine::scenes
