#include <SFML/Graphics.hpp>

class GameObject {
public:
	virtual void draw(sf::RenderWindow& window) = 0;
};



int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 768), "PG Invaders");
	

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();

		window.display();
	}
	return 0;
}