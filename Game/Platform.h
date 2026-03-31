#pragma once

#include <SFML/Graphics.hpp>

enum class CollisionType
{
    ReverseX,
    ReverseY
};

class Platform
{
public:
    Platform(float x, float y, float width, float height, CollisionType collisionType);

    void draw(sf::RenderWindow& window) const;

    bool intersects(const sf::FloatRect& bounds) const;
    CollisionType getCollisionType() const { return type; }
    
    //Getters and setters
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
private:
    sf::RectangleShape shape;
    CollisionType type;
};