#include "Hitbox.h"

Hitbox::Hitbox(float x, float y, float width, float height)
{
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(255, 0, 0, 100)); // Semi-transparent red
    shape.setPosition(sf::Vector2f(x, y));
}

void Hitbox::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}