#include <SFML/Graphics.hpp>
#include <iostream>
#include "HUD.h"
#include <random>
#include <chrono>

class GameObject {
public:
    virtual void draw(sf::RenderWindow& window) = 0;
};
class Bullet : public GameObject {
private:
    sf::Vector2f position;
    sf::Text bulletObject;
    float size;
    float speed;
    bool alive;
    int weight;
    bool weightIsSet = false;
    bool fireDirection; //true - w górę, false - w dół
public:
    Bullet(sf::Vector2f position, float size, float speed, bool fireDirection) :
        position(position), size(size), speed(speed), alive(true), fireDirection(fireDirection) {
       
    }
   
    void draw(sf::RenderWindow& window) override {
        sf::Font font;
        if (!font.loadFromFile("fonts/Arimo-Regular.ttf")) {
            std::cerr << "Error loading font";
        }
        std::mt19937 r{ static_cast<std::mt19937::result_type>(
        std::chrono::steady_clock::now().time_since_epoch().count()
        ) };
        std::uniform_int_distribution<int> range{ 4,5 };

        if (!weightIsSet)
        {
            weightIsSet = true;
            weight = (int)range(r);
        }
        bulletObject = sf::Text(std::to_string(weight), font, size);
        fireDirection ? bulletObject.setFillColor(sf::Color::Yellow) : bulletObject.setFillColor(sf::Color::Red);
        bulletObject.setPosition(position);
        window.draw(bulletObject);
        
    }

    void update(float dt) {
        
        position.y -= speed * dt * fireDirection;
        if (fireDirection) //Leci w górę
        {
            if (position.y + size < 0) {
                alive = false;
            }
        }
        else if (!fireDirection) {//Leci w dół
            if (position.y - size < 0) {
                alive = false;
            }
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
        bullets.push_back(Bullet(sf::Vector2f((currentPosition.x + size / 2)+ bulletSize/2, currentPosition.y), bulletSize, bulletSpeed, true));
    }
    void resetBullets() {
        bullets.clear();
    }
};
class Enemy :public GameObject {
private:
private:
    float leftBorder;
    float rightBorder;
    float speedMultiplier;
    bool moving;
    float size;
    sf::Vector2f currentPosition;
    sf::Vector2f startingPosition;
    float velocity;
    const float MAX_VELOCITY = 300;
    float dx = 300;
    int amountOfEnemies;
    int xIndex;
    int yIndex;
public:
    std::vector<Bullet> bullets;
    Enemy(float size, float speedMultiplier, sf::RenderWindow& window, Borders& border, int amountOfEnemies, int xIndex, int yIndex) :
        size(size), speedMultiplier(speedMultiplier),
        currentPosition(sf::Vector2f(rightBorder * 0.9 - (int(size) * xIndex * amountOfEnemies * 0.2), window.getSize().y * 0.1 + (yIndex * int(size) * 4))),
        leftBorder(window.getSize().x * 0.25 + border.getBorderThickness()),
        rightBorder(window.getSize().x * 0.75 - border.getBorderThickness() - size),
        velocity(0),
        moving(false),
        amountOfEnemies(amountOfEnemies),
        xIndex(xIndex),
        yIndex(yIndex)
    {}
    void draw(sf::RenderWindow& window) override {
        sf::CircleShape enemyIcon(size);
        enemyIcon.setFillColor(sf::Color::Red);
        enemyIcon.setPosition(currentPosition);
        window.draw(enemyIcon);
    }
    bool isMoving() { return moving; }
    void startMoving() {
        moving = true;
    }
    void stopMoving() {
        velocity = 0;
        moving = false;
    }
    void moveLeft() {

    }
    void moveRight() {

    }
    void moveDown() {

    }
    void fire() {

    }
    void update(float dt) {
        float newX = currentPosition.x + velocity * dt * 0.7;

        if (newX >= leftBorder && newX + size <= rightBorder) {
            currentPosition.x = newX;
        }
        else {
            velocity = 0;
        }
    }
};