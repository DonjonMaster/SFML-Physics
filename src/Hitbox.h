#pragma once

#include <SFML/Graphics.hpp>

#include "Ball.h"
#include "Platform.h"

void checkCollision(Ball& ball, const Platform& wall)
{
    if (!wall.intersects(ball.getBounds()))
    {
        return;
    }
    if (wall.getCollisionType() == CollisionType::ReverseX)
    {
        ball.invertSpeedX();
    }
    else
    {
        ball.invertSpeedY();
    }
}