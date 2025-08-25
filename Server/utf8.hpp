#pragma once

#include <string>

std::ostream& operator<<(std::ostream& os, const char32_t& chr);

// UTF -> Unicode 转换器（只读）
class Utf8Wrapper {
    std::string content;
public:
    class Utf8ConstIter {
        const Utf8Wrapper* wrapper;
        size_t position;
        friend class Utf8Iter;
    public:
        Utf8ConstIter(const Utf8Wrapper& utf8);
        Utf8ConstIter(const Utf8Wrapper& utf8, size_t pos);
        Utf8ConstIter(const Utf8ConstIter& other);
        Utf8ConstIter& operator=(const Utf8ConstIter& other);
        Utf8ConstIter(Utf8ConstIter&& other);
        Utf8ConstIter& operator=(Utf8ConstIter&& other);
        Utf8ConstIter& operator++();
        Utf8ConstIter operator++(int);
        char32_t operator*();
        bool operator!=(const Utf8ConstIter& other) const;
        bool operator==(const Utf8ConstIter& other) const;
    };
    Utf8Wrapper(std::string str);
    Utf8ConstIter begin() const;
    Utf8ConstIter end() const;
    Utf8ConstIter cbegin() const;
    Utf8ConstIter cend() const;
};

std::string toString(char32_t t);