#pragma once

#include <string>

// A越小越透明
class Color {
    unsigned char r_, g_, b_, a_;
public:
    const static Color BLACK;
    const static Color RED;
    const static Color GREEN;
    const static Color YELLOW;
    const static Color BLUE;
    const static Color PURPLE;
    const static Color CYAN;
    const static Color WHITE;
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xff);
    Color(std::string str);
    Color(const char* str);
    short r() const;
    short g() const;
    short b() const;
    short a() const;
};
