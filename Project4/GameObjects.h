#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "HUD.h"
class GameObject {
public:
	virtual void draw(sf::RenderWindow& window) = 0;
};
class Player :public GameObject {
private:
	float leftBorder;
	float rightBorder;
	float speedMultiplier;
	float dx;
	int scores;
	float size;
	sf::Vector2f currentPosition;
public:
	int getScores() { return scores; }
	void setSpeedMultiplier(float speedMultiplier) { this->speedMultiplier = speedMultiplier; }
	void setSize(float size) { this->size = size; }
	Player(float size, float speedMultiplier, sf::RenderWindow& window, Borders& border) :
		size(size), speedMultiplier(speedMultiplier), dx(50), scores(0), 
		currentPosition(sf::Vector2f((window.getSize().x / 2) - int(size), window.getSize().y*0.9)),
		leftBorder(window.getSize().x * 0.25+border.getBorderThickness()),
		rightBorder(window.getSize().x * 0.75-border.getBorderThickness()-size)
	{}
	void draw(sf::RenderWindow& window) override {
		sf::CircleShape playerIcon(size);
		playerIcon.setFillColor(sf::Color::White);
		playerIcon.setPosition(currentPosition);
		window.draw(playerIcon);
	}
	void moveLeft(float dx){
		if (currentPosition.x - (dx * speedMultiplier) >= leftBorder)
		{
			currentPosition = sf::Vector2f(currentPosition.x-(dx*speedMultiplier),currentPosition.y);
		}
	}
	void moveRight(float dx){
		if (currentPosition.x + (dx * speedMultiplier) + size <= rightBorder)
		{
			currentPosition = sf::Vector2f(currentPosition.x + (dx * speedMultiplier), currentPosition.y);
		}
	}
	void fire(){}
};
