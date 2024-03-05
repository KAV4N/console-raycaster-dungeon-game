
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <math.h>

class Player {
private:
    const double minElapsedTime = 0.01f;
    double fPlayerX;
    double fPlayerY;
    double fPlayerAngle;
    double fPlayerSpeed;
    double fResetToZeroAngle();
    bool isValidPosition(double x, double y, const std::wstring& map, int nMapHeight, int nMapWidth) const;

public:
    Player(); 
    double getPlayerX() const;
    double getPlayerY() const;
    double getPlayerAngle() const;
    double getPlayerSpeed() const;

    void setPlayerX(double x);
    void setPlayerY(double y);
    void setPlayerAngle(double angle);
    void setPlayerSpeed(double speed);

    void rotateLeft(double fElapsedTime);
    void rotateRight(double fElapsedTime);
    void moveFront(double fElapsedTime, const std::wstring& map, int nMapHeight, int nMapWidth);
    void moveBack(double fElapsedTime, const std::wstring& map, int nMapHeight, int nMapWidth);
    

    
};

#endif