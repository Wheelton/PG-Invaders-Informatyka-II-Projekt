﻿#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class HudElement {
	virtual void draw(sf::RenderWindow& window) = 0;
};
class Timer {
private:
	sf::Clock clock;
	int totalSeconds;
	int currentSeconds;
public:
	Timer(int seconds) : totalSeconds(seconds) {}

	void update() {
		sf::Time elapsed = clock.getElapsedTime();
		currentSeconds = static_cast<int>(elapsed.asSeconds());
	}
	void resetTimer() {
		clock.restart();
	}
	int getCurrentSeconds() const {
		return currentSeconds;
	}
};
class Borders : public HudElement {
private:
	float borderThickness;
	sf::Color borderColor;
	sf::Color bgColor;
public:
	void draw(sf::RenderWindow& window) override {
		sf::Vector2f windowSize = sf::Vector2f(window.getSize().x/2, window.getSize().y);
		sf::RectangleShape border(windowSize);
		border.setFillColor(bgColor);
		border.setOutlineColor(borderColor);
		border.setOutlineThickness(borderThickness);
		border.setPosition(windowSize.x - windowSize.x * 0.5, 0);
		window.draw(border);
	}
	Borders(float borderThickness, sf::Color borderColor) :
		 borderThickness(borderThickness), borderColor(borderColor) {
		if (borderColor == sf::Color::White)
		{
			bgColor = sf::Color::Black;
		} else if (borderColor == sf::Color::Black)
		{
			bgColor = sf::Color::White;
		}
	}
	float getBorderThickness() { return borderThickness; }
};
class LeftContent:public HudElement{
private:
	Borders border;
	static const int LEFT_CONTENT_ITEMS_COUNT = 5;
	sf::Font font;
	sf::Text text[LEFT_CONTENT_ITEMS_COUNT];
public:
	LeftContent(Borders border, sf::RenderWindow& window):border(border){
		if (!font.loadFromFile("fonts/Arimo-Regular.ttf")) {
			std::cerr << "Error loading font";
		}
		float width = window.getSize().x*0.25 - border.getBorderThickness();
		float height = window.getSize().y;
		
		float spaceBetweenLines = 0.2;
		float modifier = 0.8;
		int fontSize = 20;
		//Elementy info
		text[0].setFont(font);
		text[0].setCharacterSize(fontSize);
		text[0].setFillColor(sf::Color::White);
		text[0].setString(L"← - idź w lewo");
		text[0].setPosition(sf::Vector2f(width / 2 - 150, height / 2 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		text[1].setFont(font);
		text[1].setCharacterSize(fontSize);
		text[1].setFillColor(sf::Color::White);
		text[1].setString(L"→ - idź w prawo");
		text[1].setPosition(sf::Vector2f(width / 2 - 150, height / 2 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		text[2].setFont(font);
		text[2].setCharacterSize(fontSize);
		text[2].setFillColor(sf::Color::White);
		text[2].setString("Spacja - strzelaj");
		text[2].setPosition(sf::Vector2f(width / 2 - 150, height / 2 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		text[3].setFont(font);
		text[3].setCharacterSize(fontSize);
		text[3].setFillColor(sf::Color::White);
		text[3].setString("R - restart gry");
		text[3].setPosition(sf::Vector2f(width / 2 - 150, height / 2 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		text[4].setFont(font);
		text[4].setCharacterSize(fontSize);
		text[4].setFillColor(sf::Color::White);
		text[4].setString(L"Backspace - wróć do meni");
		text[4].setPosition(sf::Vector2f(width / 2 - 150, height / 2 * modifier + spaceBetweenLines));
		

	}
	void draw(sf::RenderWindow& window) override {
		sf::RectangleShape leftContentBg(sf::Vector2f(window.getSize().x - (window.getSize().x * 0.75)-border.getBorderThickness(), window.getSize().y));
		leftContentBg.setFillColor(sf::Color(20,20,200,100));
		leftContentBg.setPosition(0.f, 0.f);
		window.draw(leftContentBg);
		for (int i = 0; i < LEFT_CONTENT_ITEMS_COUNT; i++)
		{
			window.draw(text[i]);
		}
	}
};
class RightContent :public HudElement {
private:
	Borders& border;
	Timer& timer;
	static const int RIGHT_CONTENT_ITEMS_COUNT = 3;
	sf::Font font;
	sf::Text text[RIGHT_CONTENT_ITEMS_COUNT];
	int scores;
public:
	RightContent(Borders& border, sf::RenderWindow& window, Timer& timer) :border(border), timer(timer) {
		if (!font.loadFromFile("fonts/Arimo-Regular.ttf")) {
			std::cerr << "Error loading font";
		}
		float rightBorderX = window.getSize().x * 0.75 + border.getBorderThickness();
		float width = window.getSize().x - (window.getSize().x * 0.75) - border.getBorderThickness();
		float height = window.getSize().y;

		float spaceBetweenLines = 0.2;
		float modifier = 0.8;
		int fontSize = 20;

		//Elementy info
		text[0].setFont(font);
		text[0].setCharacterSize(fontSize);
		text[0].setFillColor(sf::Color::White);
		text[0].setString("Czas: "+std::to_string(0)+"s");
		text[0].setPosition(sf::Vector2f(rightBorderX + width / 2 - 150, height / 2 * modifier + spaceBetweenLines));

		modifier = modifier + spaceBetweenLines;
		text[1].setFont(font);
		text[1].setCharacterSize(fontSize);
		text[1].setFillColor(sf::Color::White);
		text[1].setString("Punkty: " + std::to_string(scores));
		text[1].setPosition(sf::Vector2f(rightBorderX + width / 2 - 150, height / 2 * modifier + spaceBetweenLines));

		modifier = modifier + spaceBetweenLines;
		text[2].setFont(font);
		text[2].setCharacterSize(fontSize);
		text[2].setFillColor(sf::Color::White);
		text[2].setString("Wrogi: " + std::to_string(0));
		text[2].setPosition(sf::Vector2f(rightBorderX + width / 2 - 150, height / 2 * modifier + spaceBetweenLines));

	}
	void draw(sf::RenderWindow& window) override {
		sf::RectangleShape rightContentBg(sf::Vector2f(window.getSize().x - (window.getSize().x * 0.75) - border.getBorderThickness(), window.getSize().y));
		rightContentBg.setFillColor(sf::Color(20, 20, 200, 100));
		rightContentBg.setPosition(window.getSize().x*0.75+border.getBorderThickness(), 0.f);
		window.draw(rightContentBg);
		for (int i = 0; i < RIGHT_CONTENT_ITEMS_COUNT; i++)
		{
			window.draw(text[i]);
		}
	}
	void updateTimer() {
		text[0].setString("Czas: " + std::to_string(timer.getCurrentSeconds()) + "s");
	}
	void updateScores(int scores) {
		this->scores = scores;
		text[1].setString("Punkty: " + std::to_string(scores));
	}
	void resetScores(int& scores) {
		scores = 0;
		this->scores = 0;
	}
	int getScores() { return scores; }
};
class HUD:public HudElement {
private:
	Borders& border;
	LeftContent& leftContent;
	RightContent& rightContent;
public:
	HUD(Borders& border, LeftContent& leftContent,RightContent& rightContent):
		border(border),leftContent(leftContent),rightContent(rightContent){}
	void draw(sf::RenderWindow& window) override {
		border.draw(window);
		leftContent.draw(window);
		rightContent.draw(window);
	}
};