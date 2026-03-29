// Programme de test SFML 3.0.2 : ouvre une fenetre et anime un cercle.
#include <SFML/Graphics.hpp>
#include <optional>
#include <array>
#include "Ball.h"
#include "Platform.h"

int main()
{
	constexpr float windowWidth = 900.f;
	constexpr float windowHeight = 600.f;
	constexpr float wallThickness = 16.f;

	sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(windowWidth), static_cast<unsigned int>(windowHeight)}), "Physics Test");
	window.setFramerateLimit(60);

	Ball ball(80.f, 260.f, 40.f);

	std::array<Platform, 4> walls = {
		Platform(0.f, 0.f, wallThickness, windowHeight, CollisionType::ReverseX),
		Platform(windowWidth - wallThickness, 0.f, wallThickness, windowHeight, CollisionType::ReverseX),
		Platform(0.f, 0.f, windowWidth, wallThickness, CollisionType::ReverseY),
		Platform(0.f, windowHeight - wallThickness, windowWidth, wallThickness, CollisionType::ReverseY)
	};

	sf::Clock clock;

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		const float dt = clock.restart().asSeconds();
		ball.update(dt);

		for (const Platform& wall : walls)
		{
			if (!wall.intersects(ball.getBounds()))
			{
				continue;
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


		window.clear();
         
		for (const Platform& wall : walls)
		{
			wall.draw(window);
		}

		ball.draw(window);

		window.display();
	}

	return 0;
}
