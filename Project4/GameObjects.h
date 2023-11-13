#include <SFML/Graphics.hpp>
#include <iostream>
#include "HUD.h"

class GameObject {
public:
    virtual void draw(sf::RenderWindow& window) = 0;
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
    Player(float size, float speedMultiplier, sf::RenderWindow& window, Borders& border) :
        size(size), speedMultiplier(speedMultiplier),
        currentPosition(sf::Vector2f((window.getSize().x / 2) - int(size), window.getSize().y * 0.9)),
        leftBorder(window.getSize().x * 0.25 + border.getBorderThickness()),
        rightBorder(window.getSize().x * 0.75 - border.getBorderThickness()),
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

    void fire(sf::RenderWindow& window) {
        
    }

};