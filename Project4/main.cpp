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
const static short int FIRE_COOLDOWN = 2;

void showMainMenu(bool& timerStarted, Menu& menu, sf::RenderWindow& window) {
	timerStarted = false;
	menu.draw(window);
}

void runGame(
	HUD& hud, sf::RenderWindow& window, RightContent& rightContent,
	Timer& timer, Player& player, bool& timerStarted, sf::Clock& frameClock,
	std::vector<Enemy>& enemyList
) {
	if (!timerStarted)
	{
		player.resetBullets();
		timer.resetTimer();
		timerStarted = true;
	}
	timer.update();
	rightContent.updateTimer();
	hud.draw(window);
	sf::Time elapsed = frameClock.restart();
	float dt = elapsed.asSeconds();
	player.update(dt);
	for (auto& bullet : player.bullets) {
		bullet.draw(window);
	}
	for (auto& enemy : enemyList)
	{
		enemy.draw(window);
	}
	player.draw(window);
}

void restartGame(
	HUD& hud, bool& timerStarted, sf::RenderWindow& window, 
	RightContent& rightContent, Timer& timer, Player& player, 
	sf::Clock frameClock, std::vector<Enemy>& enemyList
	) {
	timerStarted = false;
	player.stopMoving();
	player.resetPosition(window);
	player.resetBullets();
	runGame(hud, window, rightContent, timer, player, timerStarted, frameClock, enemyList);
}


int main()
{
	//Włącz polsie znaki w CMD
	system("chcp 65001");
	system("cls");

	sf::RenderWindow window(sf::VideoMode(1280, 768), "PG Invaders");
	int gameDifficulty = 2;
	int scores;
	bool timerStarted = false;
	float fireCooldownMultiplier = 1.0;
	int enemyAmountBasedOnDifficulty[] = {1,10,20,30};
	

	Borders border(5.f, sf::Color::White);
	LeftContent leftContent(border, window);
	Timer timer(0);
	sf::Clock frameClock;
	sf::Clock fireCooldown;

	RightContent rightContent(border, window, timer);
	HUD hud(border, leftContent, rightContent);
	Player player(25.f,1, window, border);
	std::vector<Enemy> enemyList;
	//Tworzymy listę wrogów o wybranej trudności gry z ustawień
	for (int i = 0; i < enemyAmountBasedOnDifficulty[2]/4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			enemyList.push_back(Enemy(15.f, 1, window, border, enemyAmountBasedOnDifficulty[gameDifficulty],j,i));
		}
	}

	window.setFramerateLimit(60);
	Menu menu(window.getSize().x, window.getSize().y, gameDifficulty);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (gameState == 0)
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
				else if (gameState != 0) {
					if (event.key.code == sf::Keyboard::Backspace)
					{
						std::cout << "Powrót do meni!\n";
						gameState = 0;
					}
					if (gameState == 1)
					{
						if (event.key.code == sf::Keyboard::R)
						{
							std::cout << "Restart!\n";
							restartGame(hud, timerStarted, window, rightContent, timer, player, frameClock, enemyList);
						}
						else if (event.key.code == sf::Keyboard::Left && !player.isMoving()) {
							std::cout << "Idź w lewo!\n";
							player.moveLeft();
						}
						else if (event.key.code == sf::Keyboard::Right && !player.isMoving()) {
							std::cout << "Idź w prawo!\n";
							player.moveRight();
						}
						else if (event.key.code == sf::Keyboard::Space && fireCooldown.getElapsedTime().asSeconds() > FIRE_COOLDOWN * fireCooldownMultiplier) {
							fireCooldown.restart();
							std::cout << "Strzelaj!\n";
							player.fire();
						}
					}
					if (gameState == 2)
					{
						if(event.key.code == sf::Keyboard::Left)
						{
							std::cout << "Ułatw grę!\n";
							menu.lowerDifficulty(gameDifficulty);
						}
						else if (event.key.code == sf::Keyboard::Right) {
							std::cout << "Utrudnij grę!\n";
							menu.higherDifficulty(gameDifficulty);
						}
					}

				}
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
					player.stopMoving();
				}
			}
		}
		window.clear();
		if (gameState == 0)
		{
			showMainMenu(timerStarted, menu, window);
		}
		else if (gameState == 1) {
			runGame(hud, window, rightContent, timer, player, timerStarted, frameClock, enemyList);
		}
		else {
			menu.drawSubMenu(window, gameState);
		}
			
		window.display();
	}
	return 0;
}
