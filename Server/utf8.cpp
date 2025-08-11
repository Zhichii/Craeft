#include "utf8.hpp"
#include <vector>
#include <ostream>
#include <stdexcept>

std::ostream& operator<<(std::ostream& os, const char32_t& chr) {
    std::vector<char> utf8_bytes;
    if (chr <= 0x7F) {
        utf8_bytes.push_back((char)(chr));
    }
    else if (chr <= 0x7FF) {
        utf8_bytes.push_back((char)(0b11000000 | ((chr >> 6) & 0b00011111)));
        utf8_bytes.push_back((char)(0b10000000 | ((chr) & 0b00111111)));
    }
    else if (chr <= 0xFFFF) {
        utf8_bytes.push_back((char)(0b11100000 | ((chr >> 12) & 0b00001111)));
        utf8_bytes.push_back((char)(0b10000000 | ((chr >> 6) & 0b00111111)));
        utf8_bytes.push_back((char)(0b10000000 | ((chr) & 0b00111111)));
    }
    else if (chr <= 0x10FFFF) {
        utf8_bytes.push_back((char)(0b11110000 | ((chr >> 18) & 0b00000111)));
        utf8_bytes.push_back((char)(0b10000000 | ((chr >> 12) & 0b00111111)));
        utf8_bytes.push_back((char)(0b10000000 | ((chr >> 6) & 0b00111111)));
        utf8_bytes.push_back((char)(0b10000000 | ((chr) & 0b00111111)));
    }
    else {
        os << "?";
        return os;
    }
    os.write(utf8_bytes.data(), utf8_bytes.size());
    return os;
}

Utf8Wrapper::Utf8ConstIter::Utf8ConstIter(const Utf8Wrapper& utf8): wrapper(&utf8), position(0) {}

Utf8Wrapper::Utf8ConstIter::Utf8ConstIter(const Utf8Wrapper& utf8, size_t pos): wrapper(&utf8) {
    if (pos > utf8.content.size()) throw std::runtime_error("Out of bound");
    position = pos;
    // 不检查==，因为end()就是pos == size。
}

Utf8Wrapper::Utf8ConstIter::Utf8ConstIter(const Utf8ConstIter& other) = default;

Utf8Wrapper::Utf8ConstIter& Utf8Wrapper::Utf8ConstIter::operator=(const Utf8ConstIter& other) = default;

Utf8Wrapper::Utf8ConstIter::Utf8ConstIter(Utf8ConstIter&& other) = default;

Utf8Wrapper::Utf8ConstIter& Utf8Wrapper::Utf8ConstIter::operator=(Utf8ConstIter&& other) = default;

Utf8Wrapper::Utf8ConstIter& Utf8Wrapper::Utf8ConstIter::operator++() { // UTF-8错误在构造时已经处理完毕，这里只判断越界错误。
    if (position > wrapper->content.size()) throw std::runtime_error("Out of bound");
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
            if (i+1 >= str.size())  throw std::runtime_error("Not valid UTF-8");
            if (((unsigned char)str[i+1] >> 6) != 0b10) throw std::runtime_error("Not valid UTF-8");
            i += 2;
            char32_t t = 0;
            t |= (unsigned char)str[i] & 0b00011111; t <<= 6;
            t |= (unsigned char)str[i+1] & 0b00111111;
            if (t >= 0xd800 && t <= 0xdfff) throw std::runtime_error("Not valid UTF-8");
            // 检查代理对。
        }
        else if (((unsigned char)str[i] >> 4) == 0b1110) {
            if (i+2 >= str.size())  throw std::runtime_error("Not valid UTF-8");
            if (((unsigned char)str[i+1] >> 6) != 0b10) throw std::runtime_error("Not valid UTF-8");
            if (((unsigned char)str[i+2] >> 6) != 0b10) throw std::runtime_error("Not valid UTF-8");
            i += 3;
        }
        else if (((unsigned char)str[i] >> 3) == 0b11110) {
            if (i+3 >= str.size())  throw std::runtime_error("Not valid UTF-8");
            if (((unsigned char)str[i+1] >> 6) != 0b10) throw std::runtime_error("Not valid UTF-8");
            if (((unsigned char)str[i+2] >> 6) != 0b10) throw std::runtime_error("Not valid UTF-8");
            if (((unsigned char)str[i+3] >> 6) != 0b10) throw std::runtime_error("Not valid UTF-8");
            i += 4;
        }
        else throw std::runtime_error("Not valid UTF-8!!");
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
