#pragma once
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
		/*sf::RectangleShape border(windowSize);
		border.setFillColor(bgColor);
		border.setOutlineColor(borderColor);
		border.setOutlineThickness(borderThickness);
		border.setPosition(windowSize.x - windowSize.x * 0.5, 0);
		window.draw(border);*/
		sf::Image bgImage;
		if (!bgImage.loadFromFile("resources/Background_v2.jpeg"))
			std::cerr << "Error! Failed to load background image!";
		sf::Sprite bgSprite;
		sf::Texture bgTexture;
		bgTexture.loadFromImage(bgImage);
		bgSprite.setTexture(bgTexture);
		bgSprite.setPosition(windowSize.x - windowSize.x * 0.5, 0);
		//bgSprite.setScale(sf::Vector2f(3.65f, 3.65f));// dla Background-changed.png
		bgSprite.setScale(sf::Vector2f(1.25f, 1.5f)); // dla Background_v2.jpeg
		window.draw(bgSprite);
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
	static const int LEFT_CONTENT_ITEMS_COUNT = 6;
	sf::Font font;
	std::string playerName;
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
		std::string playerNameLine = "Nazwa gracza:\n" + playerName;
		text[0].setString(playerNameLine);
		text[0].setPosition(sf::Vector2f(width / 2 - 150, 35));

		text[1].setFont(font);
		text[1].setCharacterSize(fontSize);
		text[1].setFillColor(sf::Color::White);
		text[1].setString(L"← - idź w lewo");
		text[1].setPosition(sf::Vector2f(width / 2 - 150, height / 3 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		text[2].setFont(font);
		text[2].setCharacterSize(fontSize);
		text[2].setFillColor(sf::Color::White);
		text[2].setString(L"→ - idź w prawo");
		text[2].setPosition(sf::Vector2f(width / 2 - 150, height / 3 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		text[3].setFont(font);
		text[3].setCharacterSize(fontSize);
		text[3].setFillColor(sf::Color::White);
		text[3].setString("Spacja - strzelaj");
		text[3].setPosition(sf::Vector2f(width / 2 - 150, height / 3 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		text[4].setFont(font);
		text[4].setCharacterSize(fontSize);
		text[4].setFillColor(sf::Color::White);
		text[4].setString("R - restart gry");
		text[4].setPosition(sf::Vector2f(width / 2 - 150, height / 3 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		text[5].setFont(font);
		text[5].setCharacterSize(fontSize);
		text[5].setFillColor(sf::Color::White);
		text[5].setString(L"Backspace - wróć do meni");
		text[5].setPosition(sf::Vector2f(width / 2 - 150, height / 3 * modifier + spaceBetweenLines));
		

	}
	void setPlayerNameLineValue(std::string name) {
		playerName = name;
		std::string playerNameLine = "Nazwa gracza:\n" + playerName;
		text[0].setString(playerNameLine);
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
	int enemyCounter = 0;
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
		text[2].setString("Wrogowie: " + std::to_string(enemyCounter));
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
	void updateScores(int& scores) {
		this->scores = scores;
		text[1].setString("Punkty: " + std::to_string(scores));
	}
	void resetScores(int& scores) {
		scores = 0;
		this->scores = 0;
	}
	int getScores() { return scores; }
	void updateEnemyCounter(int& enemyCount) {
		enemyCounter = enemyCount;
		text[2].setString("Wrogowie: " + std::to_string(enemyCounter));
	}
	void resetEnemyCounter(int& enemyCount) {
		enemyCounter = enemyCount;
	}
	int getEnemyCount() { return enemyCounter; }
	sf::Font getFont() { return font; }
};
class HUD:public HudElement {
private:
	LeftContent& leftContent;
	RightContent& rightContent;
public:
	Borders& border;
	HUD(Borders& border, LeftContent& leftContent,RightContent& rightContent):
		border(border),leftContent(leftContent),rightContent(rightContent){}
	void draw(sf::RenderWindow& window) override {
		border.draw(window);
		leftContent.draw(window);
		rightContent.draw(window);
	}
	void updateTimer() {
		rightContent.updateTimer();
	}
	void updateScores(int scores) {
		rightContent.updateScores(scores);
	}
	void resetScores(int& scores) {
		rightContent.resetScores(scores);
	}
	int getScores() { return rightContent.getScores(); }
	void updateEnemyCounter(int& enemyCount) {
		rightContent.updateEnemyCounter(enemyCount);
	}
	void resetEnemyCounter(int& enemyCount) {
		rightContent.resetEnemyCounter(enemyCount);
	}
	void setPlayerNameLine(std::string name) {
		leftContent.setPlayerNameLineValue(name);
	}
	int getEnemyCount() { return rightContent.getEnemyCount(); }
	void drawEndGame(sf::RenderWindow& window, bool playerIsWinner) {
		sf::Text winningText;
		sf::Font font;
		font.loadFromFile("fonts/Arimo-Regular.ttf");
		winningText.setFont(font);
		winningText.setCharacterSize(30);
		float textPositionX;
		if (playerIsWinner)
		{
			winningText.setFillColor(sf::Color::Green);
			winningText.setString(L"Zdałeś sesję!");
			textPositionX = window.getSize().x / 2 - 50;
		}
		else
		{
			winningText.setFillColor(sf::Color::Red);
			winningText.setString(L"\t\tNie zdałeś sesji!\nWciśnij R dla kolejnej próby");
			textPositionX = window.getSize().x / 3;
		}
		winningText.setPosition(sf::Vector2f(textPositionX, window.getSize().y/2));
		leftContent.draw(window);
		rightContent.draw(window);
		window.draw(winningText);
	}
};