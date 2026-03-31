#include "PhysicsScene.h"

#include <array>
#include <cmath>
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
namespace
{
constexpr float kWindowWidth = 1368.f;
constexpr float kWindowHeight = 768.f;
constexpr float kLevelRadius = 300.f;
constexpr float kGravity = 1000.f;
constexpr float kEntryAngleDeg = -90.f; // North (top)
constexpr float kExitAngleDeg = 90.f;   // South (bottom)
constexpr float kOpeningHalfAngleDeg = 5.f;
constexpr float kBallRadius = 20.f;

sf::Vector2f levelCenter()
{
    return {kWindowWidth * 0.5f, kWindowHeight * 0.5f};
}

sf::Vector2f entryDirection()
{
    const float entryAngleRad = kEntryAngleDeg * 3.14159265358979323846f / 180.f;
    return {std::cos(entryAngleRad), std::sin(entryAngleRad)};
}

sf::Vector2f inwardDirection()
{
    const sf::Vector2f dir = entryDirection();
    return {-dir.x, -dir.y};
}

sf::Vector2f entryPoint()
{
    return levelCenter() + entryDirection() * (kLevelRadius - 45.f);
}

sf::Color moverColor(int index)
{
    static const std::array<sf::Color, 8> colors{
        sf::Color(200, 200, 255),
        sf::Color(255, 120, 120),
        sf::Color(120, 255, 120),
        sf::Color(255, 220, 120),
        sf::Color(120, 220, 255),
        sf::Color(220, 120, 255),
        sf::Color(255, 170, 210),
        sf::Color(170, 255, 220)
    };

    return colors[static_cast<std::size_t>(index) % colors.size()];
}

void spawnMoverInternal(Scene& scene, int index)
{
    auto mover = std::make_unique<GameObject>("Mover" + std::to_string(index));
    const sf::Vector2f inDir = inwardDirection();
    const sf::Vector2f spawn = entryPoint() + inDir * (14.f * static_cast<float>(index % 6));
    const sf::Vector2f vel = inDir * (210.f + 8.f * static_cast<float>(index % 5))
        + sf::Vector2f((index % 3 - 1) * 30.f, (index % 4 - 1.5f) * 24.f);

    mover->addComponent<TransformComponent>(spawn);
    mover->addComponent<CircleRendererComponent>(kBallRadius, moverColor(index));
    mover->addComponent<VelocityComponent>(vel);
    mover->addComponent<GravityComponent>(kGravity);
    mover->addComponent<OrbitWallsBounceComponent>(&scene, std::vector<std::string>{"LevelCircle"});
    scene.pushObject(std::move(mover));
}
} // namespace

std::unique_ptr<Scene> createPhysicsScene()
{
    // Create scene
    auto scene = std::make_unique<Scene>();

    // Create one internal circular wall (hollow circle where balls bounce inside)
    auto pushInternalCircleWall = [&](const std::string& name,
                                      sf::Vector2f position,
                                      float radius,
                                      sf::Color color,
                                      float entryAngleDeg,
                                      float exitAngleDeg,
                                      float openingHalfAngleDeg)
    {
        auto wall = std::make_unique<GameObject>(name);
        wall->addComponent<TransformComponent>(position);
        auto& circleWall = wall->addComponent<CircleWallComponent>(radius, color, 8.f, true);  // Internal
        circleWall.setOpenings(entryAngleDeg, exitAngleDeg, openingHalfAngleDeg);
        scene->pushObject(std::move(wall));
    };

    // Single large hollow-circle level
    const sf::Vector2f center = levelCenter();
    pushInternalCircleWall("LevelCircle",
                           center,
                           kLevelRadius,
                           sf::Color(100, 150, 220),
                           kEntryAngleDeg,
                           kExitAngleDeg,
                           kOpeningHalfAngleDeg);

    for (int i = 1; i <= 16; ++i)
    {
        spawnMoverInternal(*scene, i);
    }

    return scene;
}

void spawnMoverAtEntry(Scene& scene)
{
    static int spawnCounter = 2000;
    spawnMoverInternal(scene, spawnCounter++);
}
} // namespace engine::scenes
