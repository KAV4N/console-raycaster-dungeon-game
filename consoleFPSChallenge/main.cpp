
#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

#include <Windows.h>
#include "player.h"
#include "sprite.h"


#include <math.h>

#include <thread>
#include <vector>


int nScreenWidth = 120;
int nScreenHeight = 40;
int nMapWidth = 16;
int nMapHeight = 16;

double fFOV = M_PI / 2.0f;
double fMaxRenderDist = 13.0f;
bool pokus = false;



double NormalizeAngle(double angle) {
    double two_pi = 2 * M_PI;
    double four_pi = 4 * M_PI;
    

    if (angle > four_pi) {
        angle -= two_pi;
    }
    else if (angle < two_pi) {
        angle += two_pi;
    }
    
    return angle;
}

int main() {
    const std::string brickWallPath = "Sprites/brickWall.txt";


    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  
    COORD bufferSize = { static_cast<SHORT>(nScreenWidth), static_cast<SHORT>(nScreenHeight) };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, nScreenWidth - 1, nScreenHeight - 1 };

    CHAR_INFO* screenBuff = new CHAR_INFO[nScreenWidth * nScreenHeight];
    memset(screenBuff, 0, sizeof(CHAR_INFO) * (nScreenWidth * nScreenHeight));

    
    Sprite* wallSprite = new Sprite(brickWallPath);
    Player* player = new Player();
       
    std::wstring map;
    map += L"1111111111111111";
    map += L"1000000000000001";
    map += L"1000000000000001";
    map += L"1000000010000001";
    map += L"1000000000000001";
    map += L"1000000000000001";
    map += L"1000100000000001";
    map += L"1000000010000001";
    map += L"1000000010011111";
    map += L"1000111110000001";
    map += L"1000100010000001";
    map += L"1000100010000001";
    map += L"1000000011100111";
    map += L"1000000000000001";
    map += L"1000000000000001";
    map += L"1111111111111111";

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();   
    
    const int nFrameRate = 60;
    const double fMinFrameTime = 1.0f / nFrameRate;

    std::chrono::duration<double> elapsedTime;
    double fElapsedTime = 0;

    while (true) {
        tp2 = std::chrono::system_clock::now();
        elapsedTime = tp2 - tp1;
        tp1 = tp2;

        fElapsedTime = elapsedTime.count();

 
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
            player->rotateLeft(fElapsedTime);
        }
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
            player->rotateRight(fElapsedTime);
        }
        if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
            player->moveFront(fElapsedTime, map, nMapHeight, nMapWidth);
        }
        if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
            player->moveBack(fElapsedTime, map, nMapHeight, nMapWidth);
        }
        

        for (int x = 0; x < nScreenWidth; x++) {

            double fPlayerX = player->getPlayerX();
            double fPlayerY = player->getPlayerY();

            double fSampleX = 0.0f;
            double fSampleY = 0.0f;
            
            if (fPlayerX < 1 || fPlayerY < 1) {
                int a = 10; //just for testing, in here i set breakpoint and got this value: 8.266e-40#DEN it should be this
            }
            double fPlayerAngle = player->getPlayerAngle();
            double fRayAngle = NormalizeAngle((fPlayerAngle - fFOV / 2.0f) + ((double)x / (double)nScreenWidth) * fFOV);
            
            

            float fRayDirXY[2] = { cosf(fRayAngle), sinf(fRayAngle) };
      
            double vRayStepSizeXY[2] = {
               (fRayDirXY[0] == 0) ? 1e30 : abs(1.0f / fRayDirXY[0]),
               (fRayDirXY[1] == 0) ? 1e30 : abs(1.0f / fRayDirXY[1])
            };

           
            bool bHit = false;
            double fCurDistance = 0.0f;

            double fMapCheck[2] = { fPlayerX, fPlayerY };
            double fRayLengthXY[2] = { 0 };
            double fRayStepXY[2] = { 0 };

         
            for (int i = 0; i < 2; i++) {
                if (fRayDirXY[i] < 0) {
                    fRayStepXY[i] = -1;
                    fRayLengthXY[i] = fabs((i == 0 ? fPlayerX - floor(fMapCheck[i]) : fPlayerY - floor(fMapCheck[i]))) * vRayStepSizeXY[i];
                }
                else {
                    fRayStepXY[i] = 1;
                    fRayLengthXY[i] = fabs((i == 0 ? ceil(fMapCheck[i]) - fPlayerX : ceil(fMapCheck[i]) - fPlayerY)) * vRayStepSizeXY[i];
                }
            }


            short nShade = ' ';
            short color = FG_BLACK;
            int idx = 0;
            //DDA algo
            while (!bHit && fCurDistance < fMaxRenderDist){
                
                idx = (fRayLengthXY[0] < fRayLengthXY[1]) ? 0 : 1;
                fMapCheck[idx] += fRayStepXY[idx];
                fCurDistance = fRayLengthXY[idx];
                fRayLengthXY[idx] += vRayStepSizeXY[idx];

                if ((int)fMapCheck[0] < 0 || (int)fMapCheck[0] >= nMapWidth || (int)fMapCheck[1] < 0 || (int)fMapCheck[1] >= nMapHeight) {
                    bHit = true;
                    fCurDistance = fMaxRenderDist;
                }
                else if (map.c_str()[(int)fMapCheck[1] * nMapWidth + (int)fMapCheck[0]] != L'0') {
                    bHit = true;
                    
                    // int 4 line segments
                    double fBlockMidX = (int)fMapCheck[0] + 0.5f;
                    double fBlockMidY = (int)fMapCheck[1] + 0.5f;

                    double fTestPointX = fPlayerX + fRayDirXY[0] * fCurDistance;
                    double fTestPointY = fPlayerY + fRayDirXY[1] * fCurDistance;

                    float fTestAngle = atan2f((fTestPointY - fBlockMidY), (fTestPointX - fBlockMidX));

                    if (fTestAngle >= -M_PI * 0.25f && fTestAngle < M_PI * 0.25f)
                        fSampleX = fTestPointY - (int)fMapCheck[1];
                    if (fTestAngle >= M_PI * 0.25f && fTestAngle < M_PI * 0.75f)
                        fSampleX = fTestPointX - (int)fMapCheck[0];
                    if (fTestAngle < -M_PI * 0.25f && fTestAngle >= -M_PI * 0.75f)
                        fSampleX = fTestPointX - (int)fMapCheck[0];
                    if (fTestAngle >= M_PI * 0.75f || fTestAngle < -M_PI * 0.75f)
                        fSampleX = fTestPointY - (int)fMapCheck[1];
                
                } 
            }
            float fCorrectedDistance = fCurDistance * cosf(fPlayerAngle - fRayAngle);
            int nCeiling = ((int)fCorrectedDistance != 0) ? (double)(nScreenHeight / 2.0) - nScreenHeight / ((double)fCorrectedDistance) : 0;
            int nFloor = nScreenHeight - nCeiling;
            /*
            // Loop for ceiling
            for (int y = 0; y < nCeiling && y < nScreenHeight; y++) {
                screenBuff[y * nScreenWidth + x].Char.UnicodeChar = L'-';
                screenBuff[y * nScreenWidth + x].Attributes = FG_DARK_GREY;
            }

            // Loop for walls
            for (int y = nCeiling; y < nFloor && y < nScreenHeight; y++) {
                double fSampleY = ((double)y - (double)nCeiling) / ((double)nFloor - (double)nCeiling);

                screenBuff[y * nScreenWidth + x].Char.UnicodeChar = L'1';
                screenBuff[y * nScreenWidth + x].Attributes = FG_BLACK;

                //screenBuff[y * nScreenWidth + x].Char.UnicodeChar = wallSprite->sampleGlyph(fSampleX, fSampleY);
                //screenBuff[y * nScreenWidth + x].Attributes = wallSprite->sampleColor(fSampleX, fSampleY);
            }

            // Loop for floor
            for (int y = nFloor; y < nScreenHeight; y++) {
                screenBuff[y * nScreenWidth + x].Char.UnicodeChar = L'%';
                screenBuff[y * nScreenWidth + x].Attributes = FG_DARK_GREY;
            }
            */

            for (int y = 0; y < nScreenHeight; y++){
                if (y <= nCeiling) {
                    screenBuff[y * nScreenWidth + x].Char.UnicodeChar = ' ';
                    screenBuff[y * nScreenWidth + x].Attributes = FG_DARK_GREY;
                }
                    
                else if (y > nCeiling && y <= nFloor) {
                    double fSampleY = ((double)y - (double)nCeiling) / ((double)nFloor - (double)nCeiling);

                    screenBuff[y * nScreenWidth + x].Char.UnicodeChar = wallSprite->sampleGlyph(fSampleX, fSampleY);
                    screenBuff[y * nScreenWidth + x].Attributes = wallSprite->sampleColor(fSampleX, fSampleY);
                
                }   
                else {
                    screenBuff[y * nScreenWidth + x].Char.UnicodeChar = 0x2591;
                    screenBuff[y * nScreenWidth + x].Attributes = FG_DARK_GREY;
                }
               
            }

            
        }
        wchar_t s[256];
        //swprintf_s(s, 40, L"X=%3.2f",1.0f / fElapsedTime);
        swprintf_s(s, 100, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", player->getPlayerX(), player->getPlayerY(), player->getPlayerAngle(), 1.0f / fElapsedTime);
        SetConsoleTitle(s);
 
        //WriteConsoleOutput(hConsole, screenBuff, { (short)nScreenWidth, (short)nScreenHeight }, { 0,0 }, &rectWindow);
        WriteConsoleOutput(hConsole, screenBuff, bufferSize, bufferCoord, &writeRegion);
        tp2 = std::chrono::system_clock::now();
        elapsedTime = tp2 - tp1;
        fElapsedTime = elapsedTime.count();
        
        if (fElapsedTime < fMinFrameTime) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((fMinFrameTime - fElapsedTime) * 1000)));
        }
    }
    delete player;
    delete[] screenBuff;
    delete wallSprite;
    return 0;
}
