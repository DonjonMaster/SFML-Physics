#include "Platform.h"

Platform::Platform(float x, float y, float width, float height, CollisionType collisionType)
{
    type = collisionType;
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(type == CollisionType::ReverseX ? sf::Color(80, 200, 255) : sf::Color(255, 190, 90));
    shape.setPosition(sf::Vector2f(x, y));
}

bool Platform::intersects(const sf::FloatRect& bounds) const
{
    return shape.getGlobalBounds().findIntersection(bounds).has_value();
}

void Platform::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}