
#include "sprite.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <iomanip>
#include <codecvt>
#include <algorithm>

int random(int min, int max) {
    return min + rand() % (max - min + 1);
}
//THIS IS ALFA
Sprite::Sprite(std::string spritePath) {
    std::ifstream file(spritePath);
  
    if (file.is_open()) {
        std::string line;
        std::stringstream buffer;

        buffer << file.rdbuf();

        std::string strSprite = buffer.str();
        file.seekg(0, std::ios::beg);
        file.unsetf(std::ios_base::skipws);

        height = std::count(
            std::istream_iterator<char>(file),
            std::istream_iterator<char>(),
            '\n');

        if (!strSprite.empty() && strSprite.back() != '\n') {
            height += 1;
        }

        width = strSprite.length();
        size_t position = strSprite.find('\n');
        if (position != std::string::npos) {
            width = strSprite.substr(0, position).length();
        }



        strSprite.erase(std::remove(strSprite.begin(), strSprite.end(), '\n'), strSprite.cend());
        file.close();
        
        
        sprite = new CHAR_INFO[width * height];
        memset(sprite, 0, sizeof(CHAR_INFO) * width * height);

        convertStrToCharInfo(strSprite);

    }
    else {
        Sprite(32, 32);
    }
    
}

/*
Sprite::Sprite(std::string spritePath) {
    std::ifstream file(spritePath);

    if (file.is_open()) {
        std::string line;
        std::stringstream buffer;



        buffer << file.rdbuf();

        std::string strSprite = buffer.str();
        file.seekg(0, std::ios::beg);
        file.unsetf(std::ios_base::skipws);

        height = std::count(
            std::istream_iterator<char>(file),
            std::istream_iterator<char>(),
            '\n');

        if (!strSprite.empty() && strSprite.back() != '\n') {
            height += 1;
        }

        
        std::vector<wchar_t> hexSprite = parseUTF(strSprite);
        
        width = hexSprite.size();
        
        
        auto it = std::find(hexSprite.begin(), hexSprite.end(), '\n');
        if (it != hexSprite.end()) {
            width = std::distance(hexSprite.begin(), it);
        }

        hexSprite.erase(
        std::remove(
            hexSprite.begin(),
            hexSprite.end(),
            '\n'),
            hexSprite.end());





        strSprite.erase(std::remove(strSprite.begin(), strSprite.end(), '\n'), strSprite.cend());
        file.close();


        sprite = new CHAR_INFO[width * height];
        memset(sprite, 0, sizeof(CHAR_INFO) * width * height);

        
        convertStrToCharInfo(hexSprite);

    }
    else {
        Sprite(32, 32);
    }

}*/



/*
std::vector<wchar_t> Sprite::parseUTF(std::string strSprite) {
    std::vector<wchar_t> hexSprite;

    wchar_t nShade;
    WORD color = FG_WHITE;
    bool flag = false;
    int offsetSize = 1;
    
    std::string bitString = "";

    for (int idx = 0; idx < strSprite.length(); idx++) {
        char element = strSprite[idx];

        std::bitset<8> bitElement = element;
        if (element != '\n') {
            if (offsetSize == 1) {
                flag = true;
                for (int i = bitElement.size() - 1; i > 0; i--) {
                    if (bitElement[i] == 0) {
                        break;
                    }
                    offsetSize++;
                }
            }

            for (int idx = (flag) ? offsetSize : 2; idx < bitElement.size(); idx++) {
                char test = (char)bitElement[bitElement.size() - 1 - idx];
                if (test == '\0') {
                    bitString += '0';
                }
                else {
                    bitString += '1';
                }
            }
            flag = false;
            offsetSize--;

            if (offsetSize <= 1) {

                std::bitset<64> binary(bitString);
                unsigned int decimalNumber = binary.to_ulong();

                std::stringstream hexStream;
                hexStream << std::hex << decimalNumber;

                std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                std::wstring wideString = converter.from_bytes(hexStream.str());

                wchar_t hexValue = std::wcstoul(wideString.c_str(), nullptr, 16);
                bitString.clear();

                hexSprite.push_back(hexValue);

            }
        }
        else {
            hexSprite.push_back('\n');
            bitString.clear();
        }
        
    }
    return hexSprite;
}*/

/*
std::vector<wchar_t> Sprite::parseUTF(std::string strSprite) {
    std::vector<wchar_t> hexSprite;
    unsigned short hexWord;
    
    unsigned short offset = 0;
    for (int idx = 0; idx < strSprite.length(); idx++) {
        //unsigned char curElement = strSprite[idx];
        if (strSprite[idx] != '\n') {
            std::bitset<8> test(strSprite[idx]);
            hexWord = (strSprite[idx] & 0b11110000);
            std::bitset<8> bitHexWord(hexWord);
            unsigned short bitCountHex = bitHexWord.count();

            for (int i = 0; (i < bitCountHex) && (idx + 1 < strSprite.size()); i++) {
                hexWord <<= 6;
                idx += i;
                hexWord |= (strSprite[idx] & 0b00111111);

            }
        }
        else {
            hexWord = '\n';
        }
        
        hexSprite.push_back(hexWord);
        
    }

    return hexSprite;
}
*/


/*
std::vector<wchar_t> Sprite::parseUTF(std::string strSprite) {
    std::vector<wchar_t> hexSprite;
    unsigned short hexWord;

    unsigned short offset = 0;
    for (int idx = 0; idx < strSprite.length(); idx++) {
        //unsigned char curElement = strSprite[idx];
        if (strSprite[idx] != '\n') {
            std::bitset<8> test(strSprite[idx]);
            hexWord = (strSprite[idx] & 0b11110000);
        }
        else {
            hexWord = '\n';
        }

        hexSprite.push_back(hexWord);

    }

    return hexSprite;
}
*/
/*
void Sprite::convertStrToCharInfo(std::vector<wchar_t> hexSprite) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            sprite[y * width + x].Char.UnicodeChar = hexSprite[y * width + x];
            sprite[y * width + x].Attributes = FG_WHITE;
        }
    }
}*/

// TODO NOT TO TOUCH ALFA

void Sprite::convertStrToCharInfo(std::string strSprite) {
    wchar_t nShade;
    WORD color = FG_WHITE;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            
            
            if (strSprite[y * width + x] == '-') {
                nShade = 0x2591;
                color = FG_RED;
                
            }
           
            else if (strSprite[y * width + x] == '+') {
                nShade = 0x2593;
                color = FG_DARK_RED;
            }
               
            //else if (strSprite[y * width + x] == '-')		nShade = 0x2592;
            else if (strSprite[y * width + x] == '#') {
                nShade = 0x2591;
                color = FG_BLACK;
            }
                
            else {
                nShade = ' ';		// Too far away
            }
            
            //sprite[y * width + x].Char.UnicodeChar = strSprite[y * width + x];
            sprite[y * width + x].Char.UnicodeChar = nShade;
            sprite[y * width + x].Attributes = color;
            //sprite[y * width + x].Attributes = FG_RED;
        }
    }
}


WORD Sprite::sampleColor(double x, double y) {
    int sx = (int)(x * (float)width);
    int sy = (int)(y * (float)height-1);
    if (sx < 0 || sx >= width || sy < 0 || sy >= height)
        return FG_BLACK;
    else
        return sprite[sy * width + sx].Attributes;
}


WCHAR Sprite::sampleGlyph(double x, double y) {
    int sx = (int)(x * (float)width);
    int sy = (int)(y * (float)height-1);
    if (sx < 0 || sx >= width || sy < 0 || sy >= height)
        return L' ';
    else
        return sprite[sy * width + sx].Char.UnicodeChar;
}


void Sprite::createBrickWall() {
    std::vector<std::wstring> bricks = { L"[", L"#", L"|", L"(" };

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int randomIndex = random(0, bricks.size() - 1);

            sprite[i * width + j].Char.UnicodeChar = bricks[randomIndex][0];
            sprite[i * width + j].Attributes = mainColor;
        }
    }
}

void Sprite::deleteSprite() {
    delete[] sprite;
}
