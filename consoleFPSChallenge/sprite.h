#ifndef SPRITE_H
#define SPRITE_H

#include <Windows.h>
#include <iostream>
#include <vector>
#include <iostream>

enum COLOR {
    FG_BLACK = 0x0000,
    FG_DARK_BLUE = 0x0001,
    FG_DARK_GREEN = 0x0002,
    FG_DARK_CYAN = 0x0003,
    FG_DARK_RED = 0x0004,
    FG_DARK_MAGENTA = 0x0005,
    FG_DARK_YELLOW = 0x0006,
    FG_GREY = 0x0007,
    FG_DARK_GREY = 0x0008,
    FG_BLUE = 0x0009,
    FG_GREEN = 0x000A,
    FG_CYAN = 0x000B,
    FG_RED = 0x000C,
    FG_MAGENTA = 0x000D,
    FG_YELLOW = 0x000E,
    FG_WHITE = 0x000F,
    BG_BLACK = 0x0000,
    BG_DARK_BLUE = 0x0010,
    BG_DARK_GREEN = 0x0020,
    BG_DARK_CYAN = 0x0030,
    BG_DARK_RED = 0x0040,
    BG_DARK_MAGENTA = 0x0050,
    BG_DARK_YELLOW = 0x0060,
    BG_GREY = 0x0070,
    BG_DARK_GREY = 0x0080,
    BG_BLUE = 0x0090,
    BG_GREEN = 0x00A0,
    BG_CYAN = 0x00B0,
    BG_RED = 0x00C0,
    BG_MAGENTA = 0x00D0,
    BG_YELLOW = 0x00E0,
    BG_WHITE = 0x00F0,
};


class Sprite {
private:
    COLOR mainColor = COLOR::FG_DARK_CYAN;
    COLOR secondaryColor = COLOR::FG_BLUE;
    int width = 0;
    int height = 0;
    CHAR_INFO* sprite;
    
    void createBrickWall(); 
    void convertStrToCharInfo(std::string strSprite);
    //void convertStrToCharInfo(std::vector<wchar_t> hexSprite);
    std::vector<wchar_t> parseUTF(std::string strSprite);
public:
    Sprite(std::string spritePath);

    Sprite(int width, int height) {
        this->width = width;
        this->height = height;
        sprite = new CHAR_INFO[width * height];
        memset(sprite, 0, sizeof(CHAR_INFO) * width * height);
        createBrickWall();
    }

    

    WORD sampleColor(double x, double y);
    WCHAR sampleGlyph(double x, double y);
    
    void deleteSprite();

};

#endif