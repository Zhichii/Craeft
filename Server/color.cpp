#include "color.hpp"

#include <stdexcept>

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a): r_(r), g_(g), b_(b), a_(a) {}

Color::Color(std::string str) {
    a_  = 0xff;
    if (str.size() == 4 || str.size() == 5) {
        if (str[0] != '#') throw std::runtime_error("Not valid color (Got `" + str + "`)");
        for (int i = 1; i < str.size(); i++) {
            int val = -1;
            if ('0' <= str[i] && str[i] <= '9') val = str[i] - '0';
            if ('a' <= str[i] && str[i] <= 'f') val = str[i] - 'a' + 10;
            if ('A' <= str[i] && str[i] <= 'F') val = str[i] - 'A' + 10;
            if (val == -1) throw std::runtime_error("Not valid color (Got `" + str + "`)");
            if (i == 1) r_ = val * 16 + val;
            if (i == 2) g_ = val * 16 + val;
            if (i == 3) b_ = val * 16 + val;
            if (i == 4) a_ = val * 16 + val;
        }
        return;
    }
    if (str.size() == 7 || str.size() == 9) {
        if (str[0] != '#') throw std::runtime_error("Not valid color (Got `" + str + "`)");
        for (int i = 1; i < str.size(); i++) {
            int val = -1;
            if ('0' <= str[i] && str[i] <= '9') val = str[i] - '0';
            if ('a' <= str[i] && str[i] <= 'f') val = str[i] - 'a' + 10;
            if ('A' <= str[i] && str[i] <= 'F') val = str[i] - 'A' + 10;
            if (val == -1) throw std::runtime_error("Not valid color (Got `" + str + "`)");
            if (i == 1) r_ = val * 16;
            if (i == 2) r_ += val;
            if (i == 3) g_ = val * 16;
            if (i == 4) g_ += val;
            if (i == 5) b_ = val * 16;
            if (i == 6) b_ += val;
            if (i == 7) a_ = val * 16;
            if (i == 8) a_ += val;
        }
        return;
    }
    throw std::runtime_error("Not valid color (Got `" + str + "`)");
}

Color::Color(const char* str) : Color(std::string(str)) {}

short Color::r() const { return r_; }
short Color::g() const { return g_; }
short Color::b() const { return b_; }
short Color::a() const { return a_; }

const Color Color::BLACK = Color(0, 0, 0);
const Color Color::RED = Color(224, 0, 0);
const Color Color::GREEN = Color(0, 224, 0);
const Color Color::YELLOW  = Color(224, 224, 0);
const Color Color::BLUE = Color(0, 0, 224);
const Color Color::PURPLE = Color(224, 0, 224);
const Color Color::CYAN = Color(0, 224, 224);
const Color Color::WHITE = Color(224, 224, 224);
