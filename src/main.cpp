// Main minimal: configuration engine + push de scene.
#include "engine/Engine.h"
#include "engine/scenes/PhysicsScene.h"

int main()
{
	engine::EngineConfig config;
	config.windowSize = {900u, 600u};
	config.title = "Physics Test";
	config.framerateLimit = 60;

	engine::Engine engine(config);
	engine.pushScene(engine::scenes::createPhysicsScene());

	engine.run();

	return 0;
}
