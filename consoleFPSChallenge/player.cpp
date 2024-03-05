#define _USE_MATH_DEFINES

#include "Player.h"



Player::Player() : fPlayerX(4.5f), fPlayerY(4.5f), fPlayerAngle(2*M_PI), fPlayerSpeed(5.0f) {}

// Getter implementations
double Player::getPlayerX() const {
    return fPlayerX;
}

double Player::getPlayerY() const {
    return fPlayerY;
}

double Player::getPlayerAngle() const {
    return fPlayerAngle;
}

double Player::getPlayerSpeed() const {
    return fPlayerSpeed;
}


double Player::fResetToZeroAngle() {
    double two_pi = 2 * M_PI;
    double four_pi = 4 * M_PI;

    //fPlayerAngle = fmod(fPlayerAngle, four_pi);
    if (fPlayerAngle > four_pi) {
        fPlayerAngle -= two_pi;
    }
    else if (fPlayerAngle < two_pi) {
        fPlayerAngle += two_pi;
    }

    return fPlayerAngle;
}



// Setter implementations
void Player::setPlayerX(double x) {
    fPlayerX = x;
}

void Player::setPlayerY(double y) {
    fPlayerY = y;
}

void Player::setPlayerAngle(double angle) {
    fPlayerAngle = angle;
}

void Player::setPlayerSpeed(double speed) {
    fPlayerSpeed = speed;
}

void Player::rotateLeft(double fElapsedTime){
    fElapsedTime = std::max(fElapsedTime, minElapsedTime);
    fPlayerAngle -= (fPlayerSpeed * 0.75f) * fElapsedTime;
    fPlayerAngle = fResetToZeroAngle();
}

void Player::rotateRight(double fElapsedTime){
    fElapsedTime = std::max(fElapsedTime, minElapsedTime);
    fPlayerAngle += (fPlayerSpeed * 0.75f) * fElapsedTime;
    fPlayerAngle = fResetToZeroAngle();
    fPlayerAngle = fResetToZeroAngle();
}

void Player::moveFront(double fElapsedTime, const std::wstring& map, int nMapHeight, int nMapWidth) {
    fElapsedTime = std::max(fElapsedTime, minElapsedTime);
    double newPlayerX = fPlayerX + cosf(fPlayerAngle) * fPlayerSpeed * fElapsedTime;
    double newPlayerY = fPlayerY + sinf(fPlayerAngle) * fPlayerSpeed * fElapsedTime;
    
    if (isValidPosition(newPlayerX, newPlayerY,map, nMapHeight, nMapWidth)) {
        fPlayerX = newPlayerX;
        fPlayerY = newPlayerY;

    }
}

void Player::moveBack(double fElapsedTime, const std::wstring& map, int nMapHeight, int nMapWidth) {
    fElapsedTime = std::max(fElapsedTime, minElapsedTime);
    double newPlayerX = fPlayerX - cosf(fPlayerAngle) * fPlayerSpeed * fElapsedTime;
    double newPlayerY = fPlayerY - sinf(fPlayerAngle) * fPlayerSpeed * fElapsedTime;


    if (isValidPosition(newPlayerX, newPlayerY, map, nMapHeight, nMapWidth)) {
        fPlayerX = newPlayerX;
        fPlayerY = newPlayerY;
    }

}

bool Player::isValidPosition(double x, double y, const std::wstring& map, int nMapHeight, int nMapWidth) const {
    int mapX = static_cast<int>(x);
    int mapY = static_cast<int>(y);


    if (mapX < 0 || mapX >= nMapWidth || mapY < 0 || mapY >= nMapHeight) {
        return false;
    }

    return (map[mapY * nMapWidth + mapX] == L'0');
}


