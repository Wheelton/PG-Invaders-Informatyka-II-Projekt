#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Menu {
private:
	int selectedOption;
	
	static const int MENU_ITEMS_COUNT = 4;
	static const int SETTINGS_MENU_ITEMS_COUNT = 1;
	static const int ABOUT_MENU_ITEMS_COUNT = 1;

	sf::Font font;
	
	/*
	* menu[0-3] - meni główne
	* menu[4] - ustawienia
	* menu[5] - o grze
	*/
	sf::Text menu[MENU_ITEMS_COUNT + SETTINGS_MENU_ITEMS_COUNT + ABOUT_MENU_ITEMS_COUNT];
public:
	Menu(float width, float height) : selectedOption(0) {
		if (!font.loadFromFile("fonts/Arimo-Regular.ttf")) {
			std::cerr << "Error loading font";
		}
		float spaceBetweenLines = 0.5;
		float modifier = 0.8;
		//Elementy meni głównego
		menu[0].setFont(font);
		menu[0].setFillColor(sf::Color::Green);
		menu[0].setString("Graj");
		menu[0].setPosition(sf::Vector2f(width / 2 - 50, height / 4 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		menu[1].setFont(font);
		menu[1].setFillColor(sf::Color::White);
		menu[1].setString("Ustawienia");
		menu[1].setPosition(sf::Vector2f(width / 2 - 50, height / 4 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		menu[2].setFont(font);
		menu[2].setFillColor(sf::Color::White);
		menu[2].setString("O grze");
		menu[2].setPosition(sf::Vector2f(width / 2 - 50, height / 4 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		menu[3].setFont(font);
		menu[3].setFillColor(sf::Color::White);
		menu[3].setString("Zamknij");
		menu[3].setPosition(sf::Vector2f(width / 2 - 50, height / 4 * modifier + spaceBetweenLines));
		//Elementy meni ustawien
		modifier = 0.8;
		menu[4].setFont(font);
		menu[4].setFillColor(sf::Color::White);
		menu[4].setString(L"Tu będą ustawienia");
		menu[4].setPosition(sf::Vector2f(width / 2 - 50, height / 4 * modifier + spaceBetweenLines));
		//Elementy meni z informacją o grze
		modifier = 0.8;
		menu[5].setFont(font);
		menu[5].setFillColor(sf::Color::White);
		menu[5].setString("PG Invaders\nProjekt gra Informatyka II 2023\nAutor: Illia Shcheboruk");
		menu[5].setLineSpacing(4 * modifier + spaceBetweenLines);
		menu[5].setPosition(sf::Vector2f(width / 2 - 200, height / 4 * modifier + spaceBetweenLines));


	}
	void draw(sf::RenderWindow& window) {
		for (int i = 0; i < MENU_ITEMS_COUNT; i++)
		{
			window.draw(menu[i]);
		}
	}
	void drawSubMenu(sf::RenderWindow& window, int gameState) {
		//Wyświetl ustawienia
		if (gameState == 2)
		{
			for (int i = MENU_ITEMS_COUNT; i < MENU_ITEMS_COUNT+SETTINGS_MENU_ITEMS_COUNT; i++)
			{
				window.draw(menu[i]);
			}
		}
		else if (gameState == 3) {
			for (int i = MENU_ITEMS_COUNT + SETTINGS_MENU_ITEMS_COUNT; i < MENU_ITEMS_COUNT + SETTINGS_MENU_ITEMS_COUNT+ABOUT_MENU_ITEMS_COUNT; i++)
			{
				window.draw(menu[i]);
			}
		}
	}
	void moveUp() {
		if (selectedOption - 1 >= 0)
		{
			menu[selectedOption].setFillColor(sf::Color::White);
			selectedOption--;
			menu[selectedOption].setFillColor(sf::Color::Green);
		}
	}
	void moveDown() {
		if (selectedOption + 1 < MENU_ITEMS_COUNT)
		{
			menu[selectedOption].setFillColor(sf::Color::White);
			selectedOption++;
			menu[selectedOption].setFillColor(sf::Color::Green);
		}
	}
	int getSelectedOption() const {
		return selectedOption;
	}
};