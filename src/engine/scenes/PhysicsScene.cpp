#include "PhysicsScene.h"

#include <string>

#include "../BoxRendererComponent.h"
#include "../CircleRendererComponent.h"
#include "../GameObject.h"
#include "../SpriteBoundsBounceComponent.h"
#include "../TransformComponent.h"
#include "../VelocityComponent.h"

// #include "../ColliderComponent.h"

namespace engine::scenes
{
std::unique_ptr<Scene> createPhysicsScene()
{
    constexpr float windowWidth = 900.f;
    constexpr float windowHeight = 600.f;
    constexpr float wallThickness = 16.f;

    // Create scene
    auto scene = std::make_unique<Scene>();

    // Create walls
    auto pushWall = [&](const std::string& name, sf::Vector2f position, sf::Vector2f size, sf::Color color)
    {
        auto wall = std::make_unique<GameObject>(name);
        wall->addComponent<TransformComponent>(position);
        wall->addComponent<BoxRendererComponent>(size, color);
        // auto& collider = wall->addComponent<ColliderComponent>(size);
        // collider.setDebugDraw(true, sf::Color(255, 255, 255, 120));
        scene->pushObject(std::move(wall));
    };

    // Add walls around the edges of the window
    pushWall("WallLeft", {0.f, 0.f}, {wallThickness, windowHeight}, sf::Color(80, 200, 255));
    pushWall("WallRight", {windowWidth - wallThickness, 0.f}, {wallThickness, windowHeight}, sf::Color(80, 200, 255));
    pushWall("WallTop", {0.f, 0.f}, {windowWidth, wallThickness}, sf::Color(255, 190, 90));
    pushWall("WallBottom", {0.f, windowHeight - wallThickness}, {windowWidth, wallThickness}, sf::Color(255, 190, 90));

    // Create some moving objects
    auto pushMover = [&](const std::string& name, sf::Vector2f position, float radius, sf::Vector2f velocity, sf::Color color)
    {
        auto mover = std::make_unique<GameObject>(name);
        mover->addComponent<TransformComponent>(position);
        mover->addComponent<CircleRendererComponent>(radius, color);
        mover->addComponent<VelocityComponent>(velocity);
        mover->addComponent<SpriteBoundsBounceComponent>(
            sf::FloatRect({wallThickness, wallThickness},
                          {windowWidth - 2.f * wallThickness, windowHeight - 2.f * wallThickness}));

        // auto& velocityComp = mover->addComponent<VelocityComponent>(velocity);
        // auto& collider = mover->addComponent<ColliderComponent>(
        //     sf::Vector2f(radius * 2.f, radius * 2.f));
        //
        // // DEBUG: enable collider debug draw to visualize collision bounds
        // collider.setDebugDraw(true, sf::Color::Green);
        //
        // collider.setOnCollision([&velocityComp](GameObject& other)
        // {
        //     auto current = velocityComp.getVelocity();
        //     const std::string& otherName = other.getName();
        //
        //     if (otherName == "WallLeft" || otherName == "WallRight")
        //     {
        //         current.x = -current.x;
        //     }
        //     else if (otherName == "WallTop" || otherName == "WallBottom")
        //     {
        //         current.y = -current.y;
        //     }
        //
        //     velocityComp.setVelocity(current);
        // });

        scene->pushObject(std::move(mover));
    };

    pushMover("MoverA", {80.f, 260.f}, 20.f, {220.f, 220.f}, sf::Color(80, 180, 255));
    pushMover("MoverB", {200.f, 200.f}, 20.f, {320.f, 320.f}, sf::Color(255, 120, 120));
    pushMover("MoverC", {400.f, 100.f}, 20.f, {180.f, 280.f}, sf::Color(120, 255, 120));
    pushMover("MoverD", {300.f, 300.f}, 20.f, {280.f, 180.f}, sf::Color(255, 255, 120));

    return scene;
}
} // namespace engine::scenes
