#include "PhysicsScene.h"

#include <string>

#include "../BoxRendererComponent.h"
#include "../CircleRendererComponent.h"
#include "../GameObject.h"
#include "../OrbitWallsBounceComponent.h"
#include "../RotationComponent.h"
#include "../SpriteBoundsBounceComponent.h"
#include "../TransformComponent.h"
#include "../VelocityComponent.h"

namespace engine::scenes
{
std::unique_ptr<Scene> createPhysicsScene()
{
    constexpr float windowWidth = 1920.f;
    constexpr float windowHeight = 1080.f;
    constexpr float wallThickness = 16.f;
    constexpr float wallsRotationSpeed = 25.f;
    const sf::Vector2f wallsPivot{windowWidth * 0.5f, windowHeight * 0.5f};

    // Create scene
    auto scene = std::make_unique<Scene>();

    // Create walls
    auto pushWall = [&](const std::string& name, sf::Vector2f position, sf::Vector2f size, sf::Color color)
    {
        auto wall = std::make_unique<GameObject>(name);
        wall->addComponent<TransformComponent>(position + size * 0.5f);
        wall->addComponent<BoxRendererComponent>(size, color, size * 0.5f);
        // wall->addComponent<RotationComponent>(wallsRotationSpeed, wallsPivot);
        scene->pushObject(std::move(wall));
    };

    // Add walls around the edges of the window DO NOT REMOVE
    pushWall("WallLeft", {0.f, 0.f}, {wallThickness, windowHeight}, sf::Color(80, 200, 255));
    pushWall("WallRight", {windowWidth - wallThickness, 0.f}, {wallThickness, windowHeight}, sf::Color(80, 200, 255));
    pushWall("WallTop", {0.f, 0.f}, {windowWidth, wallThickness}, sf::Color(255, 190, 90));
    pushWall("WallBottom", {0.f, windowHeight - wallThickness}, {windowWidth, wallThickness}, sf::Color(255, 190, 90));

    // Among Us shape at center
    const float centerX = windowWidth * 0.5f;
    const float centerY = windowHeight * 0.5f;
    const float scale = 80.f;
    
    // Body (main rectangle)
    pushWall("AmongUsBody", {centerX - scale * 1.2f, centerY - scale * 0.5f}, {scale * 2.4f, scale * 1.8f}, sf::Color(200, 80, 255));
    
    // Head (top part)
    pushWall("AmongUsHeadLeft", {centerX - scale * 1.0f, centerY - scale * 2.2f}, {scale * 0.8f, scale * 0.8f}, sf::Color(200, 80, 255));
    pushWall("AmongUsHeadCenter", {centerX - scale * 0.2f, centerY - scale * 2.5f}, {scale * 0.4f, scale * 1.1f}, sf::Color(200, 80, 255));
    pushWall("AmongUsHeadRight", {centerX + scale * 0.2f, centerY - scale * 2.2f}, {scale * 0.8f, scale * 0.8f}, sf::Color(200, 80, 255));
    
    // Visor (little window on head)
    pushWall("AmongUsVisor", {centerX - scale * 0.6f, centerY - scale * 2.3f}, {scale * 1.2f, scale * 0.3f}, sf::Color(80, 255, 200));
    
    // Left leg
    pushWall("AmongUsLegLeft", {centerX - scale * 0.8f, centerY + scale * 1.3f}, {scale * 0.5f, scale * 0.9f}, sf::Color(200, 80, 255));
    
    // Right leg
    pushWall("AmongUsLegRight", {centerX + scale * 0.3f, centerY + scale * 1.3f}, {scale * 0.5f, scale * 0.9f}, sf::Color(200, 80, 255));
    
    // Backpack-like element
    pushWall("AmongUsBackpack", {centerX + scale * 1.4f, centerY - scale * 0.3f}, {scale * 0.6f, scale * 1.2f}, sf::Color(200, 80, 255));

    // Create some moving objects
    auto pushMover = [&](const std::string& name, sf::Vector2f position, float radius, sf::Vector2f velocity, sf::Color color)
    {
        auto mover = std::make_unique<GameObject>(name);
        mover->addComponent<TransformComponent>(position);
        mover->addComponent<CircleRendererComponent>(radius, color);
        mover->addComponent<VelocityComponent>(velocity);
        mover->addComponent<OrbitWallsBounceComponent>(
            scene.get(),
            std::vector<std::string>{"WallLeft", "WallRight", "WallTop", "WallBottom", 
                                      "AmongUsBody", "AmongUsHeadLeft", "AmongUsHeadCenter", "AmongUsHeadRight",
                                      "AmongUsVisor", "AmongUsLegLeft", "AmongUsLegRight", "AmongUsBackpack"});
        scene->pushObject(std::move(mover));
    };

    // Movers positioned around the screen
    pushMover("MoverA", {300.f, 300.f}, 25.f, {250.f, 250.f}, sf::Color(80, 180, 255));
    pushMover("MoverB", {1600.f, 300.f}, 25.f, {-250.f, 250.f}, sf::Color(255, 120, 120));
    pushMover("MoverC", {300.f, 800.f}, 25.f, {250.f, -250.f}, sf::Color(120, 255, 120));
    pushMover("MoverD", {1600.f, 800.f}, 25.f, {-250.f, -250.f}, sf::Color(255, 255, 120));

    return scene;
}
} // namespace engine::scenes
