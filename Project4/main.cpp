#include <SFML/Graphics.hpp>
#include <iostream>
#include "Menu.h"
#include "GameObjects.h"
#include "HUD.h"
/*
GAME STATES:
0 - Menu otwarte
1 - Opcja Graj wybrana, zacznij grę
2 - Opcja Ustawienia wybrana, pokaż ustawienia
3 - Pcja O grze wybrana, pokaż stronę o grze
*/
unsigned short int gameState = 0;

void showMainMenu(bool& timerStarted, Menu& menu, sf::RenderWindow& window) {
	timerStarted = false;
	menu.draw(window);
}

void runGame(HUD& hud, sf::RenderWindow& window, RightContent& rightContent, Timer& timer, Player& player, bool& timerStarted) {
	if (!timerStarted)
	{
		timer.resetTimer();
		timerStarted = true;
	}
	timer.update();
	rightContent.updateTimer();
	hud.draw(window);
	player.draw(window);
}

void restartGame() {

}


int main()
{
	//Włącz polsie znaki w CMD
	system("chcp 65001");
	system("cls");

	sf::RenderWindow window(sf::VideoMode(1280, 768), "PG Invaders");
	int scores;
	bool timerStarted = false;
	Borders border(5.f, sf::Color::White);
	LeftContent leftContent(border, window);
	Timer timer(0);
	RightContent rightContent(border, window, timer);
	HUD hud(border, leftContent, rightContent);
	Player player;
	window.setFramerateLimit(60);
	Menu menu(window.getSize().x, window.getSize().y);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed && gameState == 0)
			{
				if (event.key.code == sf::Keyboard::Up) {
					menu.moveUp();
				}
				else if (event.key.code == sf::Keyboard::Down) {
					menu.moveDown();
				}
				else if (event.key.code == sf::Keyboard::Return) {
					
					if (menu.getSelectedOption() == 0) {
						std::cout << "Opcja Graj wybrana!\n";
						gameState = 1;
					}
					else if (menu.getSelectedOption() == 1) {
						std::cout << "Opcja Ustawienia wybrana!\n";
						gameState = 2;
					}
					else if (menu.getSelectedOption() == 2) {
						std::cout << "Opcja O grze wybrana!\n";
						gameState = 3;
					}
					else if (menu.getSelectedOption() == 3) {
						std::cout << "Opcja Zamknij wybrana!\n";
						window.close();
					}
				}
			}
			else if (event.type == sf::Event::KeyPressed && gameState != 0) {
				if (event.key.code == sf::Keyboard::Backspace)
				{
					std::cout << "Powrót do meni!\n";
					gameState = 0;
				}
			}
		}
		window.clear();
		if (gameState == 0)
		{
			showMainMenu(timerStarted, menu, window);
		}
		else if (gameState == 1) {
			runGame(hud, window, rightContent, timer, player, timerStarted);
		}
		else {
			menu.drawSubMenu(window, gameState);
		}
			
		window.display();
	}
	return 0;
}
