#include "utf8.hpp"
#include <vector>
#include <ostream>
#include <stdexcept>

Utf8Wrapper::Utf8ConstIter::Utf8ConstIter(const Utf8Wrapper& utf8): wrapper(&utf8), position(0) {}

Utf8Wrapper::Utf8ConstIter::Utf8ConstIter(const Utf8Wrapper& utf8, size_t pos): wrapper(&utf8) {
    if (pos > utf8.content.size()) throw std::runtime_error("index out of range");
    position = pos;
    // 不检查==，因为end()就是pos == size。
}

Utf8Wrapper::Utf8ConstIter::Utf8ConstIter(const Utf8ConstIter& other) = default;

Utf8Wrapper::Utf8ConstIter& Utf8Wrapper::Utf8ConstIter::operator=(const Utf8ConstIter& other) = default;

Utf8Wrapper::Utf8ConstIter::Utf8ConstIter(Utf8ConstIter&& other) = default;

Utf8Wrapper::Utf8ConstIter& Utf8Wrapper::Utf8ConstIter::operator=(Utf8ConstIter&& other) = default;

Utf8Wrapper::Utf8ConstIter& Utf8Wrapper::Utf8ConstIter::operator++() { // UTF-8错误在构造时已经处理完毕，这里只判断越界错误。
    if (position > wrapper->content.size()) throw std::runtime_error("index out of range");
    if (((unsigned char)wrapper->content[position] >> 7) == 0b0) position += 1;
    else if (((unsigned char)wrapper->content[position] >> 5) == 0b110) position += 2;
    else if (((unsigned char)wrapper->content[position] >> 4) == 0b1110) position += 3;
    else if (((unsigned char)wrapper->content[position] >> 3) == 0b11110) position += 4;
    return *this;
}

Utf8Wrapper::Utf8ConstIter Utf8Wrapper::Utf8ConstIter::operator++(int){
    Utf8ConstIter ret = *this;
    ++(*this);
    return ret;
}

char32_t Utf8Wrapper::Utf8ConstIter::operator*() {
    if (position >= wrapper->content.size()) throw std::runtime_error("Out of bound");
    char32_t t = 0;
    if (((unsigned char)wrapper->content[position] >> 7) == 0b0) {
        t = (unsigned char)wrapper->content[position];
    }
    else if (((unsigned char)wrapper->content[position] >> 5) == 0b110) {
        t |= (unsigned char)wrapper->content[position] & 0b00011111; t <<= 6;
        t |= (unsigned char)wrapper->content[position+1] & 0b00111111;
    }
    else if (((unsigned char)wrapper->content[position] >> 4) == 0b1110) {
        t |= (unsigned char)wrapper->content[position] & 0b00001111; t <<= 6;
        t |= (unsigned char)wrapper->content[position+1] & 0b00111111; t <<= 6;
        t |= (unsigned char)wrapper->content[position+2] & 0b00111111;
    }
    else if (((unsigned char)wrapper->content[position] >> 3) == 0b11110) {
        t |= (unsigned char)wrapper->content[position] & 0b00000111; t <<= 6;
        t |= (unsigned char)wrapper->content[position+1] & 0b00111111; t <<= 6;
        t |= (unsigned char)wrapper->content[position+2] & 0b00111111; t <<= 6;
        t |= (unsigned char)wrapper->content[position+3] & 0b00111111;
    }
    return t;
}

bool Utf8Wrapper::Utf8ConstIter::operator!=(const Utf8ConstIter& other) const {
    return wrapper != other.wrapper || position != other.position;
}

bool Utf8Wrapper::Utf8ConstIter::operator==(const Utf8ConstIter& other) const {
    return wrapper == other.wrapper && position == other.position;
}

Utf8Wrapper::Utf8Wrapper(std::string str) {
    size_t i = 0;
    while (i < str.size()) {
        if (((unsigned char)str[i] >> 7) == 0b0) {
            i += 1;
        }
        else if (((unsigned char)str[i] >> 5) == 0b110) {
            if (i+1 >= str.size())  throw std::runtime_error("not valid utf-8");
            if (((unsigned char)str[i+1] >> 6) != 0b10) throw std::runtime_error("not valid utf-8");
            i += 2;
            // 检查代理对。
            char32_t t = 0;
            t |= (unsigned char)str[i] & 0b00011111; t <<= 6;
            t |= (unsigned char)str[i+1] & 0b00111111;
            if (t >= 0xd800 && t <= 0xdfff) throw std::runtime_error("not valid utf-8");
        }
        else if (((unsigned char)str[i] >> 4) == 0b1110) {
            if (i+2 >= str.size())  throw std::runtime_error("not valid utf-8");
            if (((unsigned char)str[i+1] >> 6) != 0b10) throw std::runtime_error("not valid utf-8");
            if (((unsigned char)str[i+2] >> 6) != 0b10) throw std::runtime_error("not valid utf-8");
            i += 3;
        }
        else if (((unsigned char)str[i] >> 3) == 0b11110) {
            if (i+3 >= str.size())  throw std::runtime_error("not valid utf-8");
            if (((unsigned char)str[i+1] >> 6) != 0b10) throw std::runtime_error("not valid utf-8");
            if (((unsigned char)str[i+2] >> 6) != 0b10) throw std::runtime_error("not valid utf-8");
            if (((unsigned char)str[i+3] >> 6) != 0b10) throw std::runtime_error("not valid utf-8");
            i += 4;
            // 检查是否超出0x10ffff。
            char32_t t;
            t |= (unsigned char)str[i] & 0b00000111; t <<= 6;
            t |= (unsigned char)str[i+1] & 0b00111111; t <<= 6;
            t |= (unsigned char)str[i+2] & 0b00111111; t <<= 6;
            t |= (unsigned char)str[i+3] & 0b00111111;
            if (t > 0x10ffff) throw std::runtime_error("not valid utf-8!");
        }
        else throw std::runtime_error("not valid utf-8!");
    }
    if (i != str.size()) throw std::runtime_error("What?");
    content = str;
}

Utf8Wrapper::Utf8ConstIter Utf8Wrapper::begin() const {
    return Utf8ConstIter(*this);
}
Utf8Wrapper::Utf8ConstIter Utf8Wrapper::end() const {
    return Utf8ConstIter(*this, content.size());
}
Utf8Wrapper::Utf8ConstIter Utf8Wrapper::cbegin() const {
    return Utf8ConstIter(*this);
}
Utf8Wrapper::Utf8ConstIter Utf8Wrapper::cend() const {
    return Utf8ConstIter(*this, content.size());
}

std::string toString(char32_t chr) {
    std::string str;
    if (chr <= 0x7F) { // 0b1111111 [7位]
        str.push_back((char)(chr));
    }
    else if (chr <= 0x7FF) { // 0b11111111111 [11位]
        str.push_back((char)(0b11000000 | ((chr >> 6) & 0b00011111)));
        str.push_back((char)(0b10000000 | ((chr) & 0b00111111)));
    }
    else if (chr <= 0xFFFF) { // 0b1111111111111111 [16位]
        str.push_back((char)(0b11100000 | ((chr >> 12) & 0b00001111)));
        str.push_back((char)(0b10000000 | ((chr >> 6) & 0b00111111)));
        str.push_back((char)(0b10000000 | ((chr) & 0b00111111)));
    }
    else if (chr <= 0x10FFFF) { // 0b100001111111111111111 [16个'1']
        str.push_back((char)(0b11110000 | ((chr >> 18) & 0b00000111)));
        str.push_back((char)(0b10000000 | ((chr >> 12) & 0b00111111)));
        str.push_back((char)(0b10000000 | ((chr >> 6) & 0b00111111)));
        str.push_back((char)(0b10000000 | ((chr) & 0b00111111)));
    }
    else return "?";
    return str;
}

std::ostream& operator<<(std::ostream& os, const char32_t& chr) {
    os << toString(chr);
    return os;
}
