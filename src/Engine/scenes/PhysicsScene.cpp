#include "PhysicsScene.h"

#include <string>

#include "../CircleRendererComponent.h"
#include "../CircleWallComponent.h"
#include "../GameObject.h"
#include "../GravityComponent.h"
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
    const float gravity = 1000.f;
    pushInternalCircleWall("LevelCircle", {centerX, centerY}, levelRadius, sf::Color(100, 150, 220));

    // Create moving objects
    auto pushMover = [&](const std::string& name, sf::Vector2f position, float radius, sf::Vector2f velocity, sf::Color color)
    {
        auto mover = std::make_unique<GameObject>(name);
        mover->addComponent<TransformComponent>(position);
        mover->addComponent<CircleRendererComponent>(radius, color);
        mover->addComponent<VelocityComponent>(velocity);
        mover->addComponent<GravityComponent>(gravity);
        
        std::vector<std::string> walls{"LevelCircle"};
        
        mover->addComponent<OrbitWallsBounceComponent>(scene.get(), walls);
        scene->pushObject(std::move(mover));
    };

    // Movers positioned inside the single circle
    pushMover("MoverA", {centerX - 180.f, centerY - 90.f}, 25.f, {220.f, -170.f}, sf::Color(80, 180, 255));
    pushMover("MoverB", {centerX + 180.f, centerY - 90.f}, 25.f, {-220.f, -170.f}, sf::Color(255, 120, 120));
    pushMover("MoverC", {centerX - 120.f, centerY + 120.f}, 25.f, {180.f, 210.f}, sf::Color(120, 255, 120));
    pushMover("MoverD", {centerX + 120.f, centerY + 120.f}, 25.f, {-180.f, 210.f}, sf::Color(255, 255, 120));
    pushMover("MoverE", {centerX, centerY - 200.f}, 25.f, {0.f, 250.f}, sf::Color(255, 120, 255));
    pushMover("MoverF", {centerX, centerY + 200.f}, 25.f, {0.f, -250.f}, sf::Color(120, 255, 255));
    pushMover("MoverG", {centerX - 200.f, centerY}, 25.f, {250.f, 0.f}, sf::Color(255, 200, 120));
    pushMover("MoverH", {centerX + 200.f, centerY}, 25.f, {-250.f, 0.f}, sf::Color(120, 200, 255));
    pushMover("MoverI", {centerX - 150.f, centerY}, 25.f, {200.f, 0.f}, sf::Color(200, 255, 120));
    pushMover("MoverJ", {centerX + 150.f, centerY}, 25.f, {-200.f, 0.f}, sf::Color(120, 255, 200));
    pushMover("MoverK", {centerX, centerY - 150.f}, 25.f, {0.f, 200.f}, sf::Color(255, 120, 200));

    for (int i = 0; i < 200; ++i)
    {
        const float angle = i * 36.f;
        const float radians = angle * 3.14159265358979323846f / 180.f;
        const float radius = levelRadius - 50.f;
        const sf::Vector2f pos = {centerX + radius * std::cos(radians), centerY + radius * std::sin(radians)};
        const sf::Vector2f vel = {-150.f * std::sin(radians), 150.f * std::cos(radians)};
        pushMover("Mover" + std::to_string(i + 1), pos, 20.f, vel, sf::Color(200, 200, 255));
    }

    return scene;
}
} // namespace engine::scenes
