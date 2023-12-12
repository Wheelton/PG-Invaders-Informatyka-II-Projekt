#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Menu.h"
#include "GameObjects.h"
#include "Game.h"
#include "HUD.h"
#include <filesystem>
class Game {
public:
	HUD& hud;
	Menu& menu;
	sf::RenderWindow& window;
	int* enemyAmountBasedOnDifficulty;
	int& gameDifficulty;
	Timer& timer;
	Player& player;
	bool& timerStarted;
	sf::Clock& playerTextureClock;
	std::vector<Enemy>& enemyList;
	int& scores; float& elapsedPlayerTextureTime;
	float& textureChangeInterval;
	sf::Clock& enemyTextureClock;
	float& elapsedEnemyTextureTime;
	int& enemyCounter;
	std::string playerName;
	sf::Clock playerFireCooldown;
	float fireCooldownMultiplier = 1.0;
	Game(HUD& hud, Menu& menu, sf::RenderWindow& window, int* enemyAmountBasedOnDifficulty, int& gameDifficulty,
		Timer& timer, Player& player, bool& timerStarted, sf::Clock& playerTextureClock,
		std::vector<Enemy>& enemyList, int& scores, float& elapsedPlayerTextureTime, float& textureChangeInterval,
		sf::Clock& enemyTextureClock, float& elapsedEnemyTextureTime, int& enemyCounter)
		:
		hud(hud), menu(menu), window(window), enemyAmountBasedOnDifficulty(enemyAmountBasedOnDifficulty), gameDifficulty(gameDifficulty),
		timer(timer),
		player(player), timerStarted(timerStarted), playerTextureClock(playerTextureClock), enemyList(enemyList),
		scores(scores), elapsedPlayerTextureTime(elapsedPlayerTextureTime), textureChangeInterval(textureChangeInterval),
		enemyTextureClock(enemyTextureClock), elapsedEnemyTextureTime(elapsedEnemyTextureTime), enemyCounter(enemyCounter)
	{}
	//Struktura dla zapisu/odczytu danych gry
	typedef struct G {
		//sf::Vector2f playerPosition;
		//int playerBulletsAmount;
		//sf::Vector2f* arrayPlayerBulletsPosition;
		//sf::Vector2f* arrEnemyPositions;
		//int enemyCount;
		std::string playerName;
		int gameDifficulty;
		int scores;
		int secondsPlayed;
		//std::vector<std::vector<sf::Vector2f>> dynamicArrayEnemyBulletsPositions;
		//Przeciążenie operatora nadpisywania do pliku
		friend std::ostream& operator<<(std::ostream& os, const G& g) {
			//os << g.playerPosition.x << ' ' << g.playerPosition.y << '\n';
			//os << g.playerBulletsAmount << '\n';
			//os << g.enemyCount << '\n';
			os << g.playerName << '\n';
			os << g.gameDifficulty << '\n';
			os << g.scores << '\n';
			os << g.secondsPlayed << '\n';
			/*if (g.playerBulletsAmount>0)
			{
				for (int i = 0; i < g.playerBulletsAmount; ++i) {
					os << g.arrayPlayerBulletsPosition[i].x << ' ' << g.arrayPlayerBulletsPosition[i].y << ' ';
				}
				os << '\n';
			}
			if (g.enemyCount>0)
			{
				for (int i = 0; i < g.enemyCount; ++i) {
					os << g.arrEnemyPositions[i].x << ' ' << g.arrEnemyPositions[i].y << ' ';
				}
				os << '\n';

				for (const auto& row : g.dynamicArrayEnemyBulletsPositions) {
					for (const auto& vec : row) {
						os << vec.x << ' ' << vec.y << ' ';
					}
					os << '\n';
				}
			}*/
			return os;
		}

		//Przeciążenie operatora odczytywania z pliku do zmiennej
		friend std::istream& operator>>(std::istream& is, G& g) {
			//is >> g.playerPosition.x >> g.playerPosition.y;

			is >> g.playerName >> g.gameDifficulty >> g.scores >> g.secondsPlayed;
			//is >> g.playerBulletsAmount >> g.enemyCount >> g.scores;
			/*if (g.playerBulletsAmount>0)
			{
				g.arrayPlayerBulletsPosition = new sf::Vector2f[g.playerBulletsAmount];
				for (int i = 0; i < g.playerBulletsAmount; ++i) {
					is >> g.arrayPlayerBulletsPosition[i].x >> g.arrayPlayerBulletsPosition[i].y;
				}

			}
			if (g.enemyCount>0)
			{
				g.arrEnemyPositions = new sf::Vector2f[g.enemyCount];

				for (int i = 0; i < g.enemyCount; ++i) {
					is >> g.arrEnemyPositions[i].x >> g.arrEnemyPositions[i].y;
				}

				// Read dynamicArrayEnemyBulletsPositions
				g.dynamicArrayEnemyBulletsPositions.clear(); // Clear the existing data
				g.dynamicArrayEnemyBulletsPositions.resize(g.enemyCount);

				for (int i = 0; i < g.enemyCount; ++i) {
					for (int j = 0; j < g.scores; ++j) {
						sf::Vector2f vec;
						is >> vec.x >> vec.y;
						g.dynamicArrayEnemyBulletsPositions[i].push_back(vec);
					}
				}

			}*/

			return is;
		}

	};
	//Zapisz dane gry
	int saveGame() {

		std::string saveFilePath = "saves/" + player.getPlayerName() + ".dat";
		std::ofstream outputFile(saveFilePath);
		int playerBulletsAmount = player.getCurrentBulletsAmount();
		sf::Vector2f* arrayPlayerBulletsPosition = player.getCurrentBulletsXY();
		int enemyAmount = enemyList.size();
		sf::Vector2f* arrayEnemyPositions = new sf::Vector2f[enemyAmount];
		int* arrayEnemyBulletsAmounts = new int[enemyAmount];
		for (int i = 0; i < enemyAmount; i++)
		{
			arrayEnemyPositions[i] = enemyList[i].getCurrentPosition();
			arrayEnemyBulletsAmounts[i] = enemyList[i].bullets.size();
			//std::cout << "enemy" << i << "(" << arrayEnemyPositions[i].x << "," << arrayEnemyPositions[i].y << ")\n";
		}
		std::vector<std::vector<sf::Vector2f>> dynamicArrayEnemyBulletsPositions(enemyAmount);
		for (int i = 0; i < enemyAmount; i++)
		{
			dynamicArrayEnemyBulletsPositions[i].resize(arrayEnemyBulletsAmounts[i]);
			//std::cout << "enemy" << i << " bul.am. " << arrayEnemyBulletsAmounts[i] << std::endl;
			for (int j = 0; j < arrayEnemyBulletsAmounts[i]; j++)
			{
				dynamicArrayEnemyBulletsPositions[i][j] = enemyList[i].bullets[j].getCurrentPosition();
				//std::cout << "bul" << j << "(" << dynamicArrayEnemyBulletsPositions[i][j].x << "," << dynamicArrayEnemyBulletsPositions[i][j].y << ")\t";
			}
			//std::cout << "\n";
		}
		//G gameInstance{player.getCurrentPosition(), playerBulletsAmount,arrayPlayerBulletsPosition, arrayEnemyPositions, enemyAmount, scores, dynamicArrayEnemyBulletsPositions };
		G gameInstance{ player.getPlayerName(), scores,timer.getCurrentSeconds() };
		if (outputFile.is_open()) {
			std::cout << "Nowy zapis gry: " << saveFilePath << "\n";
			outputFile << gameInstance;
			outputFile.close();
		}
		else {
			std::cerr << "Problem z zapisywaniem pliku:" << saveFilePath << std::endl;
			return 1;
		}
	}
	//Odczytaj dane gry
	int loadGame() {
		G loadedGameInstance;
		std::string saveFilePath = "saves/" + player.getPlayerName() + ".dat";
		std::ifstream inputFile(saveFilePath);
		if (inputFile.is_open()) {
			std::cout << "Laduje: " << saveFilePath << "\n";
			inputFile >> loadedGameInstance;
			inputFile.close();
			//std::cout << loadedGameInstance.playerName << ": Scores " << loadedGameInstance.scores << ", Time played " << loadedGameInstance.secondsPlayed << std::endl;
		}
		else {
			std::cerr << "Problem z odczytywaniem pliku:" << saveFilePath << std::endl;
			return 1;
		}

	}


	/*
		GAME STATES:
		0 - Menu otwarte
		1 - Opcja Graj wybrana, zacznij grę
		2 - Opcja Ustawienia wybrana, pokaż ustawienia
		3 - Opcja O grze wybrana, pokaż stronę o grze
		4 - Opcja podaj nazwę gracza
	*/

	int gameState = 0;
	const static short int FIRE_COOLDOWN = 2;

	void showMainMenu(bool& timerStarted, Menu& menu, sf::RenderWindow& window) {
		timerStarted = false;
		menu.draw(window);
	}
	void generateEnemyList(
		std::vector<Enemy>& enemyList,
		sf::RenderWindow& window, Borders& border, int& enemyAmount) {
		enemyList.clear();
		int enemyId = 0;
		for (int i = 0; i < enemyAmount / 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				enemyList.push_back(Enemy(enemyId, 15.f, 0.5, window, border, enemyAmount, j, i));
				enemyId++;
			}
		}
	}
	bool enemyReachedEnd(std::vector<Enemy>& enemyList, sf::RenderWindow& window) {
		bool result = false;
		for (int i = 0; i < enemyList.size(); i++)
		{
			if (enemyList[i].getCurrentPosition().y > window.getSize().y * 0.9)
				result = true;
		}
		return result;
	}

	/*
	HUD& hud, sf::RenderWindow& window,
		Timer& timer, Player& player, bool& timerStarted, sf::Clock& playerTextureClock,
		std::vector<Enemy>& enemyList, int& scores, float& elapsedPlayerTextureTime, float& textureChangeInterval,
		sf::Clock& enemyTextureClock, float& elapsedEnemyTextureTime, int& enemyCount

	*/

	void runGame() {
		//Gra tylko się zaczęła, zresetuj wszystko
		if (!timerStarted)
		{
			player.resetBullets();
			player.resetIsHit();
			timer.resetTimer();
			timerStarted = true;
		}
		//Jeżeli pocisk trafił w gracza lub wróg doszedł do pozycji gracza
		if (player.getIsHit() || enemyReachedEnd(enemyList, window))
		{
			//std::cout << player.getIsHit() << enemyReachedEnd(enemyList, window) << std::endl;
			hud.drawEndGame(window, false); //false - wyświetl ekran przegranej
		}
		else if (enemyCounter > 0)
		{
			timer.update();
			hud.updateTimer();
			hud.updateEnemyCounter(enemyCounter);
			hud.draw(window); //Odśwież i odrysuj HUD

			sf::Time elapsedPlayerTexture = playerTextureClock.restart(); //Czas od ostatniego odświeżania ekranu dla gracza
			float dtPlayer = elapsedPlayerTexture.asSeconds();
			elapsedPlayerTextureTime += dtPlayer;

			sf::Time elapsedEnemyTexture = enemyTextureClock.restart(); //Czas od ostatniego odświeżania ekranu dla wroga
			float dtEnemy = elapsedEnemyTexture.asSeconds();
			elapsedEnemyTextureTime += dtEnemy;

			// Zmień teksturę gracza co {textureChangeInterval} sekundy
			if (elapsedPlayerTextureTime >= textureChangeInterval) {
				player.updateTexture();
				elapsedPlayerTextureTime = 0.0f;  // Odśwież timer
			}
			//Rysuj każdy pocisk gracza
			for (auto& bullet : player.bullets) {
				bullet.draw(window);
			}
			//Odświeżanie tekstur wrogów
			if (elapsedEnemyTextureTime >= textureChangeInterval)
			{
				for (auto& enemy : enemyList)
				{
					enemy.updateTexture();
				}
				elapsedEnemyTextureTime = 0.0f;
			}
			//Sprawdź każdego wroga, czy gracz trafił pociskiem w niego i wyświetl wroga
			for (auto it = enemyList.begin(); it != enemyList.end();) {
				//Jeżeli wróg trafiony, usuń go
				if (it->getIsDead()) {
					it = enemyList.erase(it);
				}
				else {
					//Odśwież pozycję wroga
					it->update(dtEnemy, player.bullets, scores, enemyCounter, window);
					//Wyświetl pociski wroga
					for (auto& bullet : it->bullets) {
						bullet.draw(window);
					}
					//Sprawdź czy gracz trafiony
					player.checkIfIsHit(it->bullets, window);
					//Render wroga
					it->draw(window);
					//Idź do następnego wroga(kolejna iteracja)
					++it;
				}
			}
			//Odśwież pozycję gracza
			player.update(dtPlayer, window);
			hud.updateScores(scores);//Odśwież punkty
			player.draw(window); //Rysuj gracza
		}
		else {
			hud.drawEndGame(window, true); //true - wyświetl wygraną
			saveGame(); //Zapisz dane gry wygranej
		}
	}
	//Funkcja restart gry, odświeża wszystkie potrzebne zmienne
	void restartGame(
		HUD& hud, bool& timerStarted, sf::RenderWindow& window, Player& player, std::vector<Enemy>& enemyList, Borders& border,
		int& enemyAmount, int& scores, int gameState
	) {
		enemyList.clear();
		hud.resetScores(scores);
		hud.resetEnemyCounter(enemyAmount);
		//Jeżeli gra nie w stanie pokazywania meni
		if (gameState != 0)
		{
			generateEnemyList(enemyList, window, border, enemyAmount);
		}
		timerStarted = false;
		player.stopMoving();
		player.resetPosition(window);
		player.resetBullets();		
	}
	//Główna funkcja renderowania okna gry
	void renderGame() {
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				//Jeżeli okno zmienia rozmiar - przywróć do domyślnego
				else if (event.type == sf::Event::Resized)
					window.setSize(sf::Vector2u(1280, 768)); //1280x768 domyślnie
				if (event.type == sf::Event::KeyPressed)
				{
					//Jeżeli gra w stanie pokazywania meni
					if (gameState == 0)
					{
						if (event.key.code == sf::Keyboard::Up) {
							menu.moveUp();
						}
						else if (event.key.code == sf::Keyboard::Down) {
							menu.moveDown();
						}
						else if (event.key.code == sf::Keyboard::Enter) {

							if (menu.getSelectedOption() == 0) {
								//std::cout << "Opcja Graj wybrana!\n";
								playerName.clear();
								gameState = 4;//Zmień stan gry na pokazywanie okna wprowadzania nazwy gracza
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
								std::cout << "Opcja Tablica rekordów wybrana!\n";

							}
							else if (menu.getSelectedOption() == 4) {
								//std::cout << "Opcja Zamknij wybrana!\n";
								window.close();
							}
						}
					}
					//Jeżeli gra nie w stanie pokazywania meni
					else if (gameState != 0) {
						//Jeżeli wciśnięty przycisk powrotu
						if (event.key.code == sf::Keyboard::Backspace && gameState != 4 || gameState == 4 && event.key.code == sf::Keyboard::Escape)
						{
							gameState = 0; //Przywróć stan gry do pokazywania meni
							//"Zresetuj" licznik wrogów
							enemyCounter = enemyAmountBasedOnDifficulty[gameDifficulty];
							restartGame(hud, timerStarted, window, player, enemyList,
								hud.border, enemyCounter, scores, gameState);
							//std::cout << "Powrót do meni!\n";
						}
						//W stanie gry
						if (gameState == 1)
						{
							//Restart gry
							if (event.key.code == sf::Keyboard::R)
							{
								std::cout << "Restart!\n";
								enemyCounter = enemyAmountBasedOnDifficulty[gameDifficulty];
								restartGame(hud, timerStarted, window, player, enemyList,
									hud.border, enemyCounter, scores, gameState);
							}
							//Gracz idź w lewo
							else if (event.key.code == sf::Keyboard::Left && !player.isMoving()) {
								//std::cout << "Idź w lewo!\n";
								player.moveLeft();
							}
							//Gracz idź w prawo
							else if (event.key.code == sf::Keyboard::Right && !player.isMoving()) {
								//std::cout << "Idź w prawo!\n";
								player.moveRight();
							}
							//Gracz strzelaj
							else if (event.key.code == sf::Keyboard::Space && playerFireCooldown.getElapsedTime().asSeconds() > FIRE_COOLDOWN * fireCooldownMultiplier) {
								playerFireCooldown.restart();
								//std::cout << "Strzelaj!\n";
								player.fire();
							}
							/*else if (event.key.code == sf::Keyboard::S)
							{
								std::cout << "Zapis gry!\n";
								game.saveGame();
							}
							else if (event.key.code == sf::Keyboard::L)
							{
								std::cout << "Ładowanie gry!\n";

								game.loadGame();
							}*/

						}
						//Jeżeli gra w stanie pokazywania meni ustawień
						if (gameState == 2)
						{
							if (event.key.code == sf::Keyboard::Left)
							{
								//std::cout << "Ułatw grę!\n";
								menu.lowerDifficulty(gameDifficulty);
							}
							else if (event.key.code == sf::Keyboard::Right) {
								//std::cout << "Utrudnij grę!\n";
								menu.higherDifficulty(gameDifficulty, enemyAmountBasedOnDifficulty);
							}
						}
						//Gra w stanie wprowadzenia nazwy gracza
						if (gameState == 4)
						{
							//Jeżeli nazwa gracza długością nie mniej niż jeden znak i wciśnięty Enter
							if (event.key.code == sf::Keyboard::Enter && playerName.length() > 1)
							{
								player.setPlayerName(playerName);
								gameState = 1; //Zmień stan gry na "GRAJ"
								generateEnemyList(enemyList, window, hud.border, enemyCounter);
							}
						}

					}
				}
				//Wprowadzanie tekstu
				else if (event.type == sf::Event::TextEntered) {
					//Wprowadzenie nazwy gracza
					if (gameState == 4)
					{
						//Śledź kodów kławiszy, przyjmuj tylko litery/cyfry/spację/znaki specjalne
						if (event.text.unicode < 128 && event.text.unicode != 8 && event.text.unicode != 13 && playerName.length() < 16) {
							playerName += static_cast<char>(event.text.unicode);
						}
						//Jeżeli wciśnięty Backspace i nazwa gracza nie pusta
						else if (event.text.unicode == 8 && !playerName.empty()) {
							//Usuń znak
							playerName.pop_back();
						}
						//Odśwież pole wprowadzania nazwy gracza
						menu.updatePlayerNameInputField(playerName);
					}
				}
				//Gra w stanie "GRAJ" i przycisk jest puszczony
				else if (gameState == 1 && event.type == sf::Event::KeyReleased)
				{
					//Odpuszczono kławiszę w lewo lub prawo
					if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
						//std::cout << "Stój!\n";
						player.stopMoving();
					}
				}
			}
			window.clear();//Wyczyść okno
			//Wyświetl meni główne
			if (gameState == 0)
			{
				showMainMenu(timerStarted, menu, window);
			}
			//"Graj" grę
			else if (gameState == 1) {
				runGame();				
			}
			//Pokaż okno wprowadzania nazwy gracza
			else if (gameState == 4) {
				menu.drawPlayerNameInputField(window);
			}
			//Rysuj podmeni
			else {
				menu.drawSubMenu(window, gameState);
			}
			//Wyświetl okno
			window.display();
		}

	}
};
