#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Menu.h"
#include "GameObjects.h"
#include "Game.h"
#include "HUD.h"
#include <filesystem>


/*
	TODO:
	Dodać warunek końca gry, czyli gameover DONE
	Zapis stanu gry i odczyt ZMIENIONY CEL -> według autora nie pasuje do gry
	Zamienić tło DONE
	Dodać system usuwania wrogów i ich pocisków (dynamicznie usuwać elementy z wektorów) DONE
	Dodać kilka nowych pomocniczych napisów
	Dodać szybkie podmeni do wpisywania nazwy gracza przed wejściem do gry
	Zapis danych gry, czyli punkty, czas grania, nazwa gracza itd.
*/



int main()
{
	//Włącz polsie znaki w CMD
	system("chcp 65001");
	system("cls");

	sf::RenderWindow window(sf::VideoMode(1280, 768), "PG Invaders");
	int gameDifficulty = 1; //Trudność podstawowa
	int scores = 0;
	bool timerStarted = false;
	
	int enemyAmountBasedOnDifficulty[] = {12,20,28};
	int enemyCounter = enemyAmountBasedOnDifficulty[gameDifficulty];


	Borders border(5.f, sf::Color::White);
	Timer timer(0);

	float elapsedPlayerTextureTime = 0.0f;
	float elapsedEnemyTextureTime = 0.0f;
	float textureChangeInterval = 0.1f;

	sf::Clock playerTextureClock;
	

	sf::Clock enemyFireCooldown;
	sf::Clock enemyTextureClock;

	Player player(25.f,1, window, border);
	
	std::vector<Enemy> enemyList;

	RightContent rightContent(border, window, timer);
	LeftContent leftContent(border, window);
	HUD hud(border, leftContent, rightContent);
	
	window.setFramerateLimit(60);
	Menu menu(window.getSize().x, window.getSize().y, gameDifficulty);

	Game game(hud,menu, window, enemyAmountBasedOnDifficulty, gameDifficulty, timer, player, timerStarted,
		playerTextureClock, enemyList, scores, elapsedPlayerTextureTime,
		textureChangeInterval, enemyTextureClock, elapsedEnemyTextureTime, enemyCounter);
	
	game.renderGame();
	
	return 0;
}
