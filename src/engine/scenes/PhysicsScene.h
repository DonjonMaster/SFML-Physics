#pragma once

#include <memory>

#include "../Scene.h"

namespace engine::scenes
{
std::unique_ptr<Scene> createPhysicsScene();
}
