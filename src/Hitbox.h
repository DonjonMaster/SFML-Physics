#pragma once

#include <SFML/Graphics.hpp>

class Hitbox
{
public:
    Hitbox(float x, float y, float width, float height);

    void draw(sf::RenderWindow& window);
    
    //Getters and setters
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
private:
    sf::RectangleShape shape;
};