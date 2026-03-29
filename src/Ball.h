#pragma once

#include <SFML/Graphics.hpp>
#include "Platform.h"

class Ball
{
public:
    Ball(float x, float y, float radius, float speedX = 3.f, float speedY = 3.f);

    void update(float dt);

    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    void invertSpeedX();
    void invertSpeedY();

    //Getters and setters
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    void setPosition(const sf::Vector2f& position) { shape.setPosition(position); }
    
    float getRadius() const { return shape.getRadius(); }
    void setFillColor(const sf::Color& color) { shape.setFillColor(color); }

private:
    sf::CircleShape shape;
    sf::Vector2f speed;
};