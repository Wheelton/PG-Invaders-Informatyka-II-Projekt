#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
class Menu {
private:
	int selectedOption;
	int gameDifficulty;
	static const int MENU_ITEMS_COUNT = 5;
	static const int SETTINGS_MENU_ITEMS_COUNT = 1;
	static const int ABOUT_MENU_ITEMS_COUNT = 1;

	sf::Text playerNameInputField;
	sf::Text helpText;


	sf::Font font;

	sf::Text menu[MENU_ITEMS_COUNT + SETTINGS_MENU_ITEMS_COUNT + ABOUT_MENU_ITEMS_COUNT];
public:
	Menu(float width, float height, int gameDifficulty) : selectedOption(0), gameDifficulty(gameDifficulty) {
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
		menu[3].setString(L"Tablica rekordów");
		menu[3].setPosition(sf::Vector2f(width / 2 - 50, height / 4 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		menu[4].setFont(font);
		menu[4].setFillColor(sf::Color::White);
		menu[4].setString("Zamknij");
		menu[4].setPosition(sf::Vector2f(width / 2 - 50, height / 4 * modifier + spaceBetweenLines));
		//Elementy meni ustawien
		modifier = 0.8;
		menu[5].setFont(font);
		menu[5].setFillColor(sf::Color::Green);
		sf::String text = sf::String(L"Ustawienia:\n\nTrudność gry: ");
		text.replace(": ", ": " + std::to_string(gameDifficulty));
		menu[5].setString(text);
		menu[5].setPosition(sf::Vector2f(width / 2 - 50, height / 4 * modifier + spaceBetweenLines));
		//Elementy meni z informacją o grze
		modifier = 0.8;
		menu[6].setFont(font);
		menu[6].setFillColor(sf::Color::White);
		menu[6].setString("PG Invaders\nProjekt gra Informatyka II 2023\nAutor: Illia Shcheboruk");
		menu[6].setLineSpacing(4 * modifier + spaceBetweenLines);
		menu[6].setPosition(sf::Vector2f(width / 2 - 200, height / 4 * modifier + spaceBetweenLines));

		playerNameInputField.setFont(font);
		playerNameInputField.setFillColor(sf::Color::White);
		playerNameInputField.setPosition(sf::Vector2f(width / 2 - 200, height / 4 * 1.3));
		playerNameInputField.setString(L"Podaj nazwę gracza:\n");

		helpText.setFont(font);
		helpText.setFillColor(sf::Color::White);
		helpText.setPosition(sf::Vector2f(width / 2 - 200, height / 4 * 0.8));
		helpText.setString(L"Escape - powrót");

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
	void drawPlayerNameInputField(sf::RenderWindow& window){
		window.draw(helpText);
		window.draw(playerNameInputField);

	}
	void updatePlayerNameInputField(std::string newValue) {
		std::wstring inputText = L"Podaj nazwę gracza\n(bez polskich znaków, 2-10 symboli):\n";
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wideString = converter.from_bytes(newValue);
		playerNameInputField.setString(inputText + wideString);
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
	void lowerDifficulty(int& difficulty) {
		if (difficulty>0)
		{
			difficulty--;
			gameDifficulty = difficulty;
			sf::String text = sf::String(L"Ustawienia:\n\nTrudność gry: ");
			text.replace(": ", ": " + std::to_string(gameDifficulty));
			menu[5].setString(text);
		}
	}
	void higherDifficulty(int& difficulty, int enemyAmountBasedOnDifficulty[]) {
		int n = sizeof(enemyAmountBasedOnDifficulty) / sizeof(int);
		if (difficulty<n)
		{
			difficulty++;
			gameDifficulty = difficulty;
			sf::String text = sf::String(L"Ustawienia:\n\nTrudność gry: ");
			text.replace(": ", ": " + std::to_string(gameDifficulty));
			menu[5].setString(text);
		}
	}
	int getSelectedOption() const {
		return selectedOption;
	}
};