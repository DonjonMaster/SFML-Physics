// Main minimal: configuration engine + push de scene.
#include "Engine/Engine.h"
#include "Engine/GravityComponent.h"
#include "Engine/scenes/PhysicsScene.h"

#include <algorithm>
#include <string>

int main()
{
	engine::EngineConfig config;
	config.windowSize = {1368u, 768u};
	config.title = "Physics Test";
	config.framerateLimit = 60;

	engine::Engine engine(config);
	engine.pushScene(engine::scenes::createPhysicsScene());

	bool prevSpace = false;
	bool prevF1 = false;
	bool prevUp = false;
	bool prevDown = false;
	bool debugConsoleOpen = false;
	std::string lastMessage = "ready";

	engine.run([&](float, engine::Scene& scene) {
		const bool spacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
		const bool f1Pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1);
		const bool upPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
		const bool downPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);

		if (f1Pressed && !prevF1)
		{
			debugConsoleOpen = !debugConsoleOpen;
			lastMessage = debugConsoleOpen ? "debug on" : "debug off";
		}

		if (spacePressed && !prevSpace)
		{
			engine::scenes::spawnMoverAtEntry(scene);
			lastMessage = "spawn mover";
		}

		if (debugConsoleOpen)
		{
			if (upPressed && !prevUp)
			{
				for (const auto& object : scene.getObjects())
				{
					if (!object)
					{
						continue;
					}

					if (auto* gravity = object->getComponent<engine::GravityComponent>())
					{
						gravity->setGravity(gravity->getGravity() + 100.f);
					}
				}
				lastMessage = "gravity +100";
			}

			if (downPressed && !prevDown)
			{
				for (const auto& object : scene.getObjects())
				{
					if (!object)
					{
						continue;
					}

					if (auto* gravity = object->getComponent<engine::GravityComponent>())
					{
						gravity->setGravity(std::max(0.f, gravity->getGravity() - 100.f));
					}
				}
				lastMessage = "gravity -100";
			}
		}

		int moverCount = 0;
		float currentGravity = 0.f;
		for (const auto& object : scene.getObjects())
		{
			if (!object)
			{
				continue;
			}

			if (object->getComponent<engine::GravityComponent>())
			{
				++moverCount;
				if (currentGravity == 0.f)
				{
					currentGravity = object->getComponent<engine::GravityComponent>()->getGravity();
				}
			}
		}

		if (debugConsoleOpen)
		{
			engine.getWindow().setTitle(
				"Physics Test | DEBUG[F1] ON | Space:spawn Up/Down:gravity | movers="
				+ std::to_string(moverCount)
				+ " gravity=" + std::to_string(static_cast<int>(currentGravity))
				+ " | " + lastMessage);
		}
		else
		{
			engine.getWindow().setTitle("Physics Test | F1:debug | Space:spawn");
		}

		prevSpace = spacePressed;
		prevF1 = f1Pressed;
		prevUp = upPressed;
		prevDown = downPressed;
	});

	return 0;
}
