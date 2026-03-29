#include "Ball.h"

Ball::Ball(float x, float y, float radius, float speedX, float speedY)
{
    shape.setRadius(radius);
    shape.setFillColor(sf::Color(80, 180, 255));
    shape.setOutlineThickness(3.f);
    shape.setOutlineColor(sf::Color::White);
    shape.setPosition(sf::Vector2f(x, y));
    speed = sf::Vector2f(speedX, speedY);
}

void Ball::update(float dt)
{
    sf::Vector2f position = shape.getPosition(); // Get current position
    position.x += speed.x * dt * 220.f; // Move the ball based on speed and delta time
    position.y += speed.y * dt * 220.f; // Move the ball based on speed and delta time

    shape.setPosition(position);
}

void Ball::invertSpeedX()
{
    speed.x = -speed.x;
    sf::Color color = shape.getFillColor();
    shape.setFillColor(sf::Color(color.r + 100, color.g + 100, color.b + 100, 255)); // Reset alpha to fully opaque
}

void Ball::invertSpeedY()
{
    speed.y = -speed.y;
    sf::Color color = shape.getFillColor();
    shape.setFillColor(sf::Color(color.r - 100, color.g - 100, color.b - 100, 255)); // Reset alpha to fully opaque
}

void Ball::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

