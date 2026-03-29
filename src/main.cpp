// Main minimal: configuration engine + push de scene.
#include "Engine/Engine.h"
#include "Engine/scenes/PhysicsScene.h"

int main()
{
	engine::EngineConfig config;
	config.windowSize = {1920u, 1080u};
	config.title = "Physics Test";
	config.framerateLimit = 60;

	engine::Engine engine(config);
	engine.pushScene(engine::scenes::createPhysicsScene());

	engine.run();

	return 0;
}
