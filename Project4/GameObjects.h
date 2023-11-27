#include <SFML/Graphics.hpp>
#include <iostream>
#include "HUD.h"
#include <random>
#include <chrono>
#include <algorithm>

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
    sf::Color bulletColor;
public:
    Bullet(sf::Vector2f position, float size, float speed, bool fireDirection, sf::Color bulletColor) :
        position(position), size(size), speed(speed), alive(true), fireDirection(fireDirection), bulletColor(bulletColor)
    {}
   
    void draw(sf::RenderWindow& window) override {
        sf::Font font;
        if (!font.loadFromFile("fonts/Arimo-Regular.ttf")) {
            std::cerr << "Error loading font";
        }
        std::mt19937 r{ static_cast<std::mt19937::result_type>(
        std::chrono::steady_clock::now().time_since_epoch().count()
        ) };
        std::uniform_int_distribution<int> playerBUlletWeightRange{ 4,5 };
        std::uniform_int_distribution<int> enemyBUlletWeightRange{ 1,2 };
        if (!weightIsSet)
        {
            weightIsSet = true;
            fireDirection ? weight = (int)playerBUlletWeightRange(r) : weight = (int)enemyBUlletWeightRange(r);
        }
        bulletObject = sf::Text(std::to_string(weight), font, size);
        bulletObject.setFillColor(bulletColor);
        bulletObject.setPosition(position);
        window.draw(bulletObject);
        
    }

    void update(float dt) {
        
        fireDirection ? position.y -= speed * dt: position.y -= speed * dt * -1;
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
    float getSize() { return size; }
    bool isAlive() { return alive; }
    sf::Vector2f getCurrentPosition() { return position; }
};
class Player : public GameObject {
private:
    float leftBorder;
    float rightBorder;
    float speedMultiplier;
    bool moving;
    int direction;
    int textureSwitcher = 0;
    float size;
    bool isHit=false;
    bool isDead = false;
    sf::Vector2f currentPosition;
    float velocity;
    const float MAX_VELOCITY = 300;
    float dx = 300;
    bool isIdle = false;
    bool isMovingLeft = false;
    bool isMovingRight = false;
    bool isFiring = false;
    sf::Sprite playerSprite;
    sf::Texture playerTexture;
    sf::Image textureIdle;
    sf::Image textureWRight0;
    sf::Image textureWRight1;
    sf::Image textureWRight2;
    sf::Image textureWLeft0;
    sf::Image textureWLeft1;
    sf::Image textureWLeft2;
    sf::Image textureFiring;
public:
    std::vector<Bullet> bullets;
    Player(float size, float speedMultiplier, sf::RenderWindow& window, Borders& border) :
        size(size), speedMultiplier(speedMultiplier),
        currentPosition(sf::Vector2f((window.getSize().x / 2) - int(size), window.getSize().y * 0.9)),
        leftBorder(window.getSize().x * 0.25 + border.getBorderThickness()),
        rightBorder(window.getSize().x * 0.75 - border.getBorderThickness()-size),
        velocity(0),
        moving(false)
    {
        if (!textureIdle.loadFromFile("resources/Student/Student_D_Idle.png") ||
            !textureWRight0.loadFromFile("resources/Student/Student_D_Right_0.png") ||
            !textureWRight1.loadFromFile("resources/Student/Student_D_Right_1.png") ||
            !textureWRight2.loadFromFile("resources/Student/Student_D_Right_2.png") ||
            !textureWLeft0.loadFromFile("resources/Student/Student_D_Left_0.png") ||
            !textureWLeft1.loadFromFile("resources/Student/Student_D_Left_1.png") ||
            !textureWLeft2.loadFromFile("resources/Student/Student_D_Left_2.png") ||
            !textureFiring.loadFromFile("resources/Student/Student_D_Top_0.png"))
        {
            std::cerr << "Error! Failed to load player textures!\n";
        }
        playerTexture.loadFromImage(textureIdle);
    }

    void draw(sf::RenderWindow& window) override {
        playerSprite.setTexture(playerTexture);
        playerSprite.setPosition(currentPosition);
        window.draw(playerSprite);
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
    void updateTexture() {
        /*std::cout << "Direction: " << direction << ", isIdle: " << isIdle << ", isMovingLeft: " << isMovingLeft << ", isMovingRight: " << isMovingRight;
        std::cout << ", isFiring: " << isFiring << "\n";*/
        if (direction == 0 && isIdle)
        {
            playerTexture.update(textureIdle);
        }
        else if (direction == 1 && isMovingRight || direction == 2 && isMovingRight && isFiring)
        {
            if (textureSwitcher == 0){
                playerTexture.update(textureWRight0);
                textureSwitcher++;
            }
            else if (textureSwitcher == 1) {
                playerTexture.update(textureWRight1);
                textureSwitcher++;
            }
            else if (textureSwitcher == 2){
                playerTexture.update(textureWRight2);
                textureSwitcher = 0;
            }
        }
        else if (direction == -1 && isMovingLeft || direction == 2 && isMovingLeft && isFiring)
        {
            if (textureSwitcher == 0) {
                playerTexture.update(textureWLeft0);
                textureSwitcher++;
            }
            else if (textureSwitcher == 1) {
                playerTexture.update(textureWLeft1);
                textureSwitcher++;
            }
            else if (textureSwitcher == 2) {
                playerTexture.update(textureWLeft2);
                textureSwitcher = 0;
            }
        } else if (direction == 2 && isFiring)
        {
            
        }
    }
    void moveLeft() {
        if (velocity > -MAX_VELOCITY){
            isIdle = false;
            isMovingRight = false;
            isMovingLeft = true;
            isFiring = false;
            direction = -1;
            velocity = -dx * speedMultiplier;
        }
    }

    void moveRight() {
        if (velocity < MAX_VELOCITY) {
            isIdle = false;
            isMovingLeft = false;
            isMovingRight = true;
            isFiring = false;
            direction = 1;
            velocity = dx * speedMultiplier;
        }
    }
    bool isMoving() { return moving; }
    void startMoving(){
        moving = true;
    }
    void stopMoving() {
        direction = 0;
        velocity = 0;
        moving = false;
        isIdle = true;
        isFiring = false;
        isMovingLeft = false;
        isMovingRight = false;
        textureSwitcher = 0;
    }

    void fire() {
        direction = 2; 
        isFiring = true;
        float bulletSpeed = 100;
        int bulletSize = 16;
        playerTexture.loadFromImage(textureFiring);
        bullets.push_back(Bullet(sf::Vector2f((currentPosition.x + size / 2)+ bulletSize/2, currentPosition.y), bulletSize, bulletSpeed, true, sf::Color::Yellow));
    }
    void resetBullets() {
        bullets.clear();
    }
    void die() {
        isHit = true;
    }
    void checkIfIsHit(std::vector<Bullet>& enemyBullets) {
        auto condition = [&](Bullet& bullet) {
            return bullet.getCurrentPosition().x >= currentPosition.x &&
                bullet.getCurrentPosition().x <= currentPosition.x + 32 &&
                bullet.getCurrentPosition().y <= currentPosition.y + 64 &&
                bullet.getCurrentPosition().y >= currentPosition.y;
            };

        for (auto& bullet : enemyBullets)
        {
            if (condition(bullet))
            {
                die();
                std::cout << "Player is hit!\n";
            }
        }
        enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), condition), enemyBullets.end());
        
    }
};
class Enemy :public GameObject {
private:
    float leftBorder;
    float rightBorder;
    float speedMultiplier;
    bool isMoving = false;
    float size;
    sf::Vector2f currentPosition;
    sf::Vector2f startingPosition;
    float velocity;
    const float MAX_VELOCITY = 300;
    float dx = 300;
    int amountOfEnemies;
    int xIndex;
    int yIndex;
    bool changeDirection = false;
    bool isFiring = false;
    bool isHit=false;
    bool isDead = false;
    bool direction=false;//direction == false -> w lewo|direction == true -> w prawo
    int textureSwitcher = 0;
    sf::Texture enemyTexture;
    sf::Sprite enemySprite;
    sf::Image texture[3][6][3];
    int enemyColorId;
public:
    std::vector<Bullet> bullets;
    Enemy(float size, float speedMultiplier, sf::RenderWindow& window, Borders& border, int amountOfEnemies, int xIndex, int yIndex) :
        size(size), speedMultiplier(speedMultiplier),
        currentPosition(sf::Vector2f(rightBorder * 0.9 - (int(size) * xIndex * amountOfEnemies * 0.2), window.getSize().y * 0.1 + (yIndex * int(size) * 4))),
        leftBorder(window.getSize().x * 0.25 + border.getBorderThickness()),
        rightBorder(window.getSize().x * 0.75 - border.getBorderThickness() - size),
        velocity(0),
        amountOfEnemies(amountOfEnemies),
        xIndex(xIndex),
        yIndex(yIndex)
    {
        
        std::mt19937 r{ static_cast<std::mt19937::result_type>(
        std::chrono::steady_clock::now().time_since_epoch().count()
        ) };
        std::uniform_int_distribution<int> range{ 0, 2 };
        enemyColorId = (int)range(r);
        //Ładuj wszystkie tekstury
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 6; j++) {
                for (int k = 0; k < 3; k++)
                {
                    std::string path;
                    std::string enemyState[] = {"WL","WR","DL","DR","FL","FR"};
                    std::string enemyColorPath[] = { "Blue/Blue_Slime_","Green/Green_Slime_","Red/Red_Slime_" };
                   
                    path = "resources/Platformer Slimes v1.3/"+enemyColorPath[i] + enemyState[j] +"_"+ std::to_string(k) + ".png";
                  
                   
                    if (!texture[i][j][k].loadFromFile(path)) {
                        std::cerr << "Error! Failed to load enemy textures!\n";
                    }
                    
                }
            }
        }
        startMoving();
        enemyTexture.loadFromImage(texture[enemyColorId][0][0]);
    }
    void draw(sf::RenderWindow& window) override {
        enemySprite.setTexture(enemyTexture);
        if(!isFiring) 
            enemySprite.setScale(sf::Vector2f(2.0f, 2.0f));
        enemySprite.setPosition(currentPosition);
        window.draw(enemySprite);
    }
    bool getIsMoving() { return isMoving; }
    bool getIsHit() { return isHit; }
    bool getIsDead() { return isDead; }
    void startMoving() {
        isMoving = true;
    }
    void stopMoving() {
        velocity = 0;
        isMoving = false;
    }
    void moveLeft() {
        startMoving();
        direction = false;
        if (velocity > -MAX_VELOCITY) //Idź w lewo
        {
            velocity = -dx * speedMultiplier;
        }    
    }
    void moveRight() {
        startMoving();
        direction = true;
        if (velocity < MAX_VELOCITY) { //Idź w prawo
            velocity = dx * speedMultiplier;
        }
    }
    void moveDown() {
        currentPosition.y = currentPosition.y + size;
    }
    void fire() {
        std::uniform_int_distribution<> probabilities{ 1,10000 };
        std::mt19937 r{ static_cast<std::mt19937::result_type>(
        std::chrono::steady_clock::now().time_since_epoch().count()
        ) };
        int fireChance = (int)probabilities(r);
        if (fireChance < 10 && !isFiring)
        {
            float bulletSpeed = 100;
            int bulletSize = 16;
            isFiring = true;
            textureSwitcher = 0;
            sf::Color bulletColor;
            if (enemyColorId == 0)
                bulletColor = sf::Color::Blue;
            else if (enemyColorId == 1)
                bulletColor = sf::Color::Green;
            else if (enemyColorId == 2)
                bulletColor = sf::Color::Red;
            bullets.push_back(Bullet(sf::Vector2f((currentPosition.x + size / 2) + bulletSize / 2, currentPosition.y + size), bulletSize, bulletSpeed, false, bulletColor));
        }
    }
    void updateTexture() {
       
        if (!direction) //w lewo
        {
            if (isMoving && !isFiring)
            {
                enemyTexture.update(texture[enemyColorId][0][textureSwitcher]);
                textureSwitcher == 2 ? textureSwitcher = 0 : textureSwitcher++;
            }
            else if (isMoving && isFiring)
            {
                std::cout << "textureSwitcher: " << textureSwitcher << ", isMoving: " << isMoving << ", isHit: " << isHit << ", isFiring: " << isFiring << "\n";
                enemyTexture.update(texture[enemyColorId][2][textureSwitcher]);
                if (textureSwitcher == 2)
                {
                    textureSwitcher = 0;
                    isFiring = false;
                }
                else {
                    textureSwitcher++;
                }
            }
            else if (isHit) {
                enemyTexture.update(texture[enemyColorId][2][textureSwitcher]);
                if (textureSwitcher == 2)
                {
                    textureSwitcher = 0;
                    isDead = true;
                }
                else {
                    textureSwitcher++;
                }
            }
        }
        else if (direction) //w prawo
        {
            if (isMoving && !isFiring)
            {
                enemyTexture.update(texture[enemyColorId][1][textureSwitcher]);
                textureSwitcher == 2 ? textureSwitcher = 0 : textureSwitcher++;
            }
            else if (isMoving && isFiring)
            {
                std::cout << "textureSwitcher: " << textureSwitcher << ", isMoving: " << isMoving << ", isHit: " << isHit << ", isFiring: " << isFiring << "\n";
                enemyTexture.update(texture[enemyColorId][3][textureSwitcher]);
                if (textureSwitcher == 2)
                {
                    textureSwitcher = 0;
                    isFiring = false;
                }
                else {
                    textureSwitcher++;
                }
            }
            else if (isHit) {
                enemyTexture.update(texture[enemyColorId][3][textureSwitcher]);
                if (textureSwitcher == 2)
                {
                    textureSwitcher = 0;
                    isDead = true;
                }
                else {
                    textureSwitcher++;
                }
                
            }
        }
    }
    void die() {
        isHit = true;
        textureSwitcher = 0;
        stopMoving();
    }
    void update(float dt, std::vector<Bullet>& playersBullets, int& scores, int& enemyCount) {
        checkIfIsHit(dt, playersBullets, scores, enemyCount);
        fire();
        for (auto& bullet : bullets) {
            bullet.update(dt);
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& bullet) { return !bullet.isAlive(); }), bullets.end());
    }
    void checkIfIsHit(float dt, std::vector<Bullet>& playersBullets, int& scores, int& enemyCount) {
        auto condition = [&](Bullet& bullet) {
            return bullet.getCurrentPosition().x >= currentPosition.x &&
                bullet.getCurrentPosition().x <= currentPosition.x + 16 &&
                bullet.getCurrentPosition().y <= currentPosition.y + 16 &&
                bullet.getCurrentPosition().y >= currentPosition.y;
            };

        for (auto& bullet : playersBullets)
        {
            if (condition(bullet))
            {
                die();
                std::cout << "Hit!\n";
                scores += 10;
                enemyCount>0 ? enemyCount--:NULL;
            }
        }
        playersBullets.erase(std::remove_if(playersBullets.begin(), playersBullets.end(), condition), playersBullets.end());
        if (!isHit)
        {
            if (isMoving)
            {
                changeDirection ? moveRight() : moveLeft();
                float newX = currentPosition.x + velocity * dt * 0.7;
                if (newX >= leftBorder && newX + size <= rightBorder) {
                    currentPosition.x = newX;
                }
                else {
                    velocity = 0;
                    changeDirection = !changeDirection;
                    moveDown();
                }
            }
            
        }
    }
};