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

	std::vector<sf::Text> tableOfRecords;

	std::vector<float> initialRecordPositions;

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
		menu[3].setString(L"Tabela rekordów");
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
		playerNameInputField.setPosition(sf::Vector2f(width / 3 - 400, height / 4 * 1.3));
		playerNameInputField.setString(L"Podaj nazwę gracza:\n");
		playerNameInputField.setCharacterSize(20);

		helpText.setFont(font);
		helpText.setFillColor(sf::Color::White);
		helpText.setPosition(sf::Vector2f(width / 2 - 200, height / 4 * 0.8));
		helpText.setString(L"Escape - powrót");

	}
	void updateTableOfRecords(sf::RenderWindow& window, std::vector<std::string> records) {
		float width = window.getSize().x * 0.5 - 200;
		float height = window.getSize().y;

		float spaceBetweenLines = 0.2;
		float modifier = 0.8;
		int fontSize = 20;
		tableOfRecords.clear();
		initialRecordPositions.clear();
		for (auto& record:records)
		{
			sf::Text textRecord;
			//Elementy info
			textRecord.setFont(font);
			textRecord.setCharacterSize(fontSize);
			textRecord.setFillColor(sf::Color::White);
			textRecord.setString(record);
			textRecord.setPosition(sf::Vector2f(width, height / 3 * modifier + spaceBetweenLines));
			initialRecordPositions.push_back(height / 3 * modifier + spaceBetweenLines);
			modifier = modifier + spaceBetweenLines;
			tableOfRecords.push_back(textRecord);
		}
	}
	void updateTableOfRecordsOffset(sf::RenderWindow& window, float offset) {
		auto record = tableOfRecords.begin();
		auto initYPosition = initialRecordPositions.begin();


		if (tableOfRecords.size()==initialRecordPositions.size())
		{
			while (record != tableOfRecords.end() && initYPosition != initialRecordPositions.end()) {
				sf::Text& text = *record;
				float initialY = *initYPosition;

				float newYPos = initialY + offset;
			
				std::cout << newYPos << std::endl;
				text.setPosition(sf::Vector2f(text.getPosition().x, newYPos));
				++record;
				++initYPosition;
			}
		}
		else
		{
			std::vector<std::string> stringRecords;
			for (auto& record : tableOfRecords)
			{
				stringRecords.push_back(record.getString());
			}
			updateTableOfRecords(window, stringRecords);
		}
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
	void drawTableOfRecords(sf::RenderWindow& window) {
		for (auto& record : tableOfRecords)
		{
			window.draw(record);
		}
	}
	void updatePlayerNameInputField(std::string newValue) {
		std::wstring inputText =
			L"Witaj w świecie 'PG Invaders'!\n"
			L"Jako student, który poświęcił wiele czasu na naukę aż do późnych godzin nocnych, znalazłeś się na uczelni,\nby przysiąść nad książkami i przygotować się do zbliżającej się sesji.\n"
			L"Jednak w nocy, gdy wszystko wydawało się spokojne, na niebie pojawiły się złowieszcze chmury.\n"
			L"Nagle, zza zakamarków uczelnianych korytarzy, wyłoniło się zło w postaci groźnych ocen, które zagrażały studentom.\n"
			L"Twoje ulubione miejsce nauki stało się polem bitwy, a Twoim celem jest obrona uczelni przed tym niebezpieczeństwem.\n"
			L"Poruszaj się sprawnie w lewo i w prawo, używając strzałek, aby unikać zbliżających się złych ocen.\n"
			L"Twoją bronią są dobre oceny, którymi musisz celnie strzelać w kierunku wrogów, aby je zniszczyć.\n"
			L"Pamiętaj jednak, że złe oceny również stanowią zagrożenie – jeśli nie unikniesz ich ataków,\nmoże to zaszkodzić Twoim szansom na zdanie sesji!\n"
			L"Czy zdołasz obronić uczelnię, utrzymać się w grze i osiągnąć najlepszy wynik? To już zależy od Ciebie.\n"
			L"Powodzenia, studente! Wstań do walki i udowodnij, że dobre oceny są najskuteczniejszą bronią przeciwko złu sesyjnemu w 'PG Invaders'!\n"
			L"\tŻeby rozpocząć, podaj dane studenta (imię lub indeks) i wciśnij ENTER (bez polskich znaków, 2 - 10 symboli):\n";

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