#include <SFML/Graphics.hpp>
#include <iostream>

/*
GAME STATES:
0 - Idle(Menu is opened)
1 - Play selected, run the game
2 - Settings is selected, open settings
3 - About is selected, open About page
*/
unsigned short int gameState = 0;



class Menu {
private:
	int selectedOption;
	static const int MENU_ITEMS_COUNT = 4;
	sf::Font font;
	sf::Text menu[MENU_ITEMS_COUNT];
public:
	Menu(float width, float height) : selectedOption(0) {
		if (!font.loadFromFile("fonts/arial.ttf")) {
			std::cerr << "Error loading font";
		}
		float spaceBetweenLines = 0.5;
		float modifier = 0.8;

		menu[0].setFont(font);
		menu[0].setFillColor(sf::Color::Green);
		menu[0].setString("Play");
		menu[0].setPosition(sf::Vector2f(width / 2 - 50, height / 4 * modifier+spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		menu[1].setFont(font);
		menu[1].setFillColor(sf::Color::White);
		menu[1].setString("Settings");
		menu[1].setPosition(sf::Vector2f(width / 2 - 50, height / 4 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		menu[2].setFont(font);
		menu[2].setFillColor(sf::Color::White);
		menu[2].setString("About");
		menu[2].setPosition(sf::Vector2f(width / 2 - 50, height / 4 * modifier + spaceBetweenLines));
		modifier = modifier + spaceBetweenLines;
		menu[3].setFont(font);
		menu[3].setFillColor(sf::Color::White);
		menu[3].setString("Exit");
		menu[3].setPosition(sf::Vector2f(width / 2 - 50, height / 4 * modifier + spaceBetweenLines));


	}
	void draw(sf::RenderWindow& window) {
		for (int i = 0; i < MENU_ITEMS_COUNT; i++)
		{
			window.draw(menu[i]);
		}
	}
	void moveUp() {
		if (selectedOption - 1 >=0)
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


class GameObject {
public:
	virtual void draw(sf::RenderWindow& window) = 0;
};
class Player :public GameObject {
public:
	void draw(sf::RenderWindow& window) override {
		sf::CircleShape playerIcon(100.f);
		playerIcon.setFillColor(sf::Color::White);
		playerIcon.setPosition(sf::Vector2f(window.getSize().x/2, window.getSize().y/2));
		window.draw(playerIcon);
	}
};


int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 768), "PG Invaders");
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
						std::cout << "Play selected!\n";
						gameState = 1;
					}
					else if (menu.getSelectedOption() == 1) {
						std::cout << "Settings selected!\n";
						gameState = 2;
					}
					else if (menu.getSelectedOption() == 2) {
						std::cout << "About selected!\n";
						gameState = 3;
					}
					else if (menu.getSelectedOption() == 3) {
						std::cout << "Exit selected!\n";
						window.close();
					}
				}
			}
			else if (event.type == sf::Event::KeyPressed && gameState != 0) {
				if (event.key.code == sf::Keyboard::Backspace)
				{
					std::cout << "Returned to menu!\n";
					gameState = 0;
				}
			}
		}
		window.clear();
		if (gameState == 0)
		{
			menu.draw(window);
		}
		else if (gameState == 1) {
			player.draw(window);
		}
			
		window.display();
	}
	return 0;
}