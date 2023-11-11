#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class GameObject {
public:
	virtual void draw(sf::RenderWindow& window) = 0;
};
class Player :public GameObject {
public:
	void draw(sf::RenderWindow& window) override {
		sf::CircleShape playerIcon(100.f);
		playerIcon.setFillColor(sf::Color::White);
		playerIcon.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
		window.draw(playerIcon);
	}
};
