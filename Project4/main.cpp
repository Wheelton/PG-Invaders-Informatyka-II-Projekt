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
3 - Opcja O grze wybrana, pokaż stronę o grze
*/
unsigned short int gameState = 0;
const static short int FIRE_COOLDOWN = 2;

void showMainMenu(bool& timerStarted, Menu& menu, sf::RenderWindow& window) {
	timerStarted = false;
	menu.draw(window);
}
void generateEnemyList(
	std::vector<Enemy>& enemyList, 
	sf::RenderWindow& window, Borders& border, int& enemyAmount) {
	for (int i = 0; i < enemyAmount / 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			enemyList.push_back(Enemy(15.f, 0.5, window, border, enemyAmount, j, i));
		}
	}
}
void gameOver() {

}
void runGame(
	HUD& hud, sf::RenderWindow& window, RightContent& rightContent,
	Timer& timer, Player& player, bool& timerStarted, sf::Clock& playerTextureClock,
	std::vector<Enemy>& enemyList, int& scores, float& elapsedPlayerTextureTime, float& textureChangeInterval,
	sf::Clock& enemyTextureClock, float& elapsedEnemyTextureTime
) {
	//Gra tylko się zaczęła, zresetuj wszystko
	if (!timerStarted)
	{
		player.resetBullets();
		timer.resetTimer();
		timerStarted = true;
	}
	timer.update();
	rightContent.updateTimer();
	hud.draw(window); //Odśwież HUD

	sf::Time elapsedPlayerTexture = playerTextureClock.restart(); //Czas od ostatniego odświeżania ekranu dla gracza
	float dtPlayer = elapsedPlayerTexture.asSeconds();
	elapsedPlayerTextureTime += dtPlayer;

	sf::Time elapsedEnemyTexture = enemyTextureClock.restart(); //Czas od ostatniego odświeżania ekranu dla wroga
	float dtEnemy = elapsedEnemyTexture.asSeconds();
	elapsedEnemyTextureTime += dtEnemy;
	// Zmień teksturę gracza co {textureChangeInterval} sekundy
	if (elapsedPlayerTextureTime >= textureChangeInterval) {
		player.updateTexture();
		elapsedPlayerTextureTime = 0.0f;  // Reset timer
	}
	//Rysuj każdy pocisk gracza
	for (auto& bullet : player.bullets) {
		bullet.draw(window);
	}
	if (elapsedEnemyTextureTime >= textureChangeInterval)
	{
		for (auto& enemy : enemyList)
		{
			enemy.updateTexture();
		}
		elapsedEnemyTextureTime = 0.0f;
	}
	//Sprawdź każdego wroga, czy gracz trafił pociskiem w niego i wyświetl wroga
	for (auto& enemy : enemyList)
	{
		if (!enemy.getIsDead())
		{
			enemy.update(dtEnemy, player.bullets, scores);
			for (auto& bullet : enemy.bullets) {
				bullet.draw(window);
			}
			player.checkIfIsHit(enemy.bullets);
			enemy.draw(window);
		}
	}
	player.update(dtPlayer);
	rightContent.updateScores(scores);//Odśwież punkty
	player.draw(window); //Rysuj gracza
}

void restartGame(
	HUD& hud, bool& timerStarted, sf::RenderWindow& window, 
	RightContent& rightContent, Timer& timer, Player& player, 
	sf::Clock frameClock, std::vector<Enemy>& enemyList, Borders& border,
	int enemyAmount, int& scores, float& elapsedTime, float& textureChangeInterval,
	sf::Clock& enemyTextureClock, float& elapsedEnemyTextureTime, int gameState
	) {
	enemyList.clear();
	rightContent.resetScores(scores);
	if (gameState != 0)
	{
		generateEnemyList(enemyList, window, border, enemyAmount);
	}
	timerStarted = false;
	player.stopMoving();
	player.resetPosition(window);
	player.resetBullets();
	runGame(hud, window, rightContent, timer, player, timerStarted, frameClock, enemyList, scores, elapsedTime, textureChangeInterval, enemyTextureClock, elapsedEnemyTextureTime);
}


int main()
{
	//Włącz polsie znaki w CMD
	system("chcp 65001");
	system("cls");

	sf::RenderWindow window(sf::VideoMode(1280, 768), "PG Invaders");
	int gameDifficulty = 1; //Trudność podstawowa
	int scores = 0;
	bool timerStarted = false;
	float fireCooldownMultiplier = 1.0;
	int enemyAmountBasedOnDifficulty[] = {12,20,28};
	

	Borders border(5.f, sf::Color::White);
	Timer timer(0);

	float elapsedPlayerTextureTime = 0.0f;
	float elapsedEnemyTextureTime = 0.0f;
	float textureChangeInterval = 0.1f;

	sf::Clock playerTextureClock;
	sf::Clock playerFireCooldown;

	sf::Clock enemyFireCooldown;
	sf::Clock enemyTextureClock;

	Player player(25.f,1, window, border);
	std::vector<Enemy> enemyList;

	RightContent rightContent(border, window, timer);
	LeftContent leftContent(border, window);
	HUD hud(border, leftContent, rightContent);
	
	
	window.setFramerateLimit(60);
	Menu menu(window.getSize().x, window.getSize().y, gameDifficulty);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
				window.setSize(sf::Vector2u(1280, 768));
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
							//std::cout << "Opcja Graj wybrana!\n";
							//Tworzymy listę wrogów o wybranej trudności gry z ustawień
							generateEnemyList(enemyList, window, border, enemyAmountBasedOnDifficulty[gameDifficulty]);
							gameState = 1;
						}
						else if (menu.getSelectedOption() == 1) {
							//std::cout << "Opcja Ustawienia wybrana!\n";
							gameState = 2;
						}
						else if (menu.getSelectedOption() == 2) {
							//std::cout << "Opcja O grze wybrana!\n";
							gameState = 3;
						}
						else if (menu.getSelectedOption() == 3) {
							//std::cout << "Opcja Zamknij wybrana!\n";
							window.close();
						}
					}
				}
				else if (gameState != 0) {
					if (event.key.code == sf::Keyboard::Backspace)
					{
						gameState = 0;
						restartGame(hud, timerStarted, window, rightContent, timer, 
							player, playerTextureClock, enemyList, border, enemyAmountBasedOnDifficulty[gameDifficulty],
							scores, elapsedPlayerTextureTime, textureChangeInterval, enemyTextureClock, elapsedEnemyTextureTime, gameState);
						//std::cout << "Powrót do meni!\n";
					}
					if (gameState == 1)
					{
						if (event.key.code == sf::Keyboard::R)
						{
							//std::cout << "Restart!\n";
							restartGame(hud, timerStarted, window, rightContent, timer,
								player, playerTextureClock, enemyList, border, enemyAmountBasedOnDifficulty[gameDifficulty],
								scores, elapsedPlayerTextureTime, textureChangeInterval, enemyTextureClock, elapsedEnemyTextureTime, gameState);
						}
						else if (event.key.code == sf::Keyboard::Left && !player.isMoving()) {
							//std::cout << "Idź w lewo!\n";
							player.moveLeft();
						}
						else if (event.key.code == sf::Keyboard::Right && !player.isMoving()) {
							//std::cout << "Idź w prawo!\n";
							player.moveRight();
						}
						else if (event.key.code == sf::Keyboard::Space && playerFireCooldown.getElapsedTime().asSeconds() > FIRE_COOLDOWN * fireCooldownMultiplier) {
							playerFireCooldown.restart();
							//std::cout << "Strzelaj!\n";
							player.fire();
						}
					}
					if (gameState == 2)
					{
						if(event.key.code == sf::Keyboard::Left)
						{
							//std::cout << "Ułatw grę!\n";
							menu.lowerDifficulty(gameDifficulty);
						}
						else if (event.key.code == sf::Keyboard::Right) {
							//std::cout << "Utrudnij grę!\n";
							menu.higherDifficulty(gameDifficulty,enemyAmountBasedOnDifficulty);
						}
					}

				}
			}
			else if (gameState == 1 && event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
					//std::cout << "Stój!\n";
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
			runGame(hud, window, rightContent, timer, player, timerStarted, playerTextureClock, enemyList, scores, elapsedPlayerTextureTime, textureChangeInterval, enemyTextureClock, elapsedEnemyTextureTime);
		}
		else {
			menu.drawSubMenu(window, gameState);
		}
			
		window.display();
	}
	return 0;
}
