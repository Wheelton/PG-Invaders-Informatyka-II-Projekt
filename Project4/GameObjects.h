#include <SFML/Graphics.hpp>
#include <iostream>
#include "HUD.h"

class GameObject {
public:
    virtual void draw(sf::RenderWindow& window) = 0;
};
class Bullet : public GameObject {
private:
    sf::Vector2f position;
    float size;
    float speed;
    bool alive;
    int weight;
public:
    Bullet(sf::Vector2f position, float size, float speed) :
        position(position), size(size), speed(speed), alive(true) {}
    int getRandomDigit() {
        std::srand(time(nullptr));
        int randomNumber = rand() % 3;
        return randomNumber + 3;
    }
    void draw(sf::RenderWindow& window) override {
        sf::Font font;
        if (!font.loadFromFile("fonts/Arimo-Regular.ttf")) {
            std::cerr << "Error loading font";
        }
        weight = getRandomDigit();
        sf::Text bulletText(std::to_string(weight), font, size);
        bulletText.setFillColor(sf::Color::Yellow);
        bulletText.setPosition(position);
        window.draw(bulletText);
        
    }

    void update(float dt) {
        position.y -= speed * dt;
        if (position.y + size < 0) {
            alive = false;
        }
    }
    int getWeight() { return weight; }
    bool isAlive() { return alive; }
};
class Player : public GameObject {
private:
    float leftBorder;
    float rightBorder;
    float speedMultiplier;
    bool moving;
    float size;
    sf::Vector2f currentPosition;
    float velocity;
    const float MAX_VELOCITY = 300;
    float dx = 300;
public:
    std::vector<Bullet> bullets;
    Player(float size, float speedMultiplier, sf::RenderWindow& window, Borders& border) :
        size(size), speedMultiplier(speedMultiplier),
        currentPosition(sf::Vector2f((window.getSize().x / 2) - int(size), window.getSize().y * 0.9)),
        leftBorder(window.getSize().x * 0.25 + border.getBorderThickness()),
        rightBorder(window.getSize().x * 0.75 - border.getBorderThickness()-size),
        velocity(0),
        moving(false)
    {}

    void draw(sf::RenderWindow& window) override {
        sf::CircleShape playerIcon(size);
        playerIcon.setFillColor(sf::Color::White);
        playerIcon.setPosition(currentPosition);
        window.draw(playerIcon);
    }
    void resetPosition(sf::RenderWindow& window) {
        currentPosition = sf::Vector2f((window.getSize().x / 2) - int(size), window.getSize().y * 0.9);
    }
    void update(float dt) {
        float newX = currentPosition.x + velocity * dt * 0.7;

        if (newX >= leftBorder && newX + size <= rightBorder) {
            currentPosition.x = newX;
        }
        else {
            velocity = 0;
        }
        for (auto& bullet : bullets) {
            bullet.update(dt);
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& bullet) { return !bullet.isAlive(); }), bullets.end());

    }

    void moveLeft() {
        if (velocity > -MAX_VELOCITY)
        {
            velocity = -dx * speedMultiplier;
        }
    }

    void moveRight() {
        if (velocity < MAX_VELOCITY) {
            velocity = dx * speedMultiplier;
        }
    }
    bool isMoving() { return moving; }
    void startMoving(){
        moving = true;
    }
    void stopMoving() {
        velocity = 0;
        moving = false;
    }

    void fire() {
        float bulletSpeed = 100;
        int bulletSize = 16;
        bullets.push_back(Bullet(sf::Vector2f((currentPosition.x + size / 2)+ bulletSize/2, currentPosition.y), bulletSize, bulletSpeed));
    }
    void resetBullets() {
        bullets.clear();
    }
};