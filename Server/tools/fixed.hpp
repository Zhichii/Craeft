#pragma once

#include <iostream>

// 定点数。
template <int SCALE, typename BASE>
class Fixed {
private:
    BASE value;
public:
    Fixed(double value = 0) : value(value*SCALE) {}
    Fixed operator+(Fixed that) {
        Fixed result;
        result.value = this->value + that.value;
        return result;
    }
    Fixed& operator+=(Fixed that) {
        this->value += that.value;
        return *this;
    }
    Fixed operator-(Fixed that) {
        Fixed result;
        result.value = this->value - that.value;
        return result;
    }
    Fixed& operator-=(Fixed that) {
        this->value -= that.value;
        return *this;
    }
    Fixed operator*(Fixed that) {
        Fixed result;
        result.value = ((long long)this->value) * that.value / SCALE; // 运算中使用long long避免问题。
        return result;
    }
    Fixed& operator*=(Fixed that) {
        this->value = ((long long)this->value) * that.value / SCALE; // 运算中使用long long避免问题。
        return *this;
    }
    Fixed operator/(Fixed that) {
        Fixed result;
        result.value = ((long long)this->value) * SCALE / that.value;  // 运算中使用long long避免问题。
        return result;
    }
    Fixed& operator/=(Fixed that) {
        this->value = ((long long)this->value) * SCALE / that.value;  // 运算中使用long long避免问题。
        return *this;
    }
    bool operator>(Fixed that) {
        return this->value > that.value;
    }
    bool operator<(Fixed that) {
        return this->value < that.value;
    }
    bool operator==(Fixed that) {
        return this->value == that.value;
    }
    bool operator!=(Fixed that) {
        return this->value != that.value;
    }
    operator double() {
        return this->value * 1. / SCALE;
    }
    friend std::ostream& operator<<(std::ostream& os, const Fixed& obj) {
        os << obj.value * 1. / SCALE;
        return os;
    }
};

typedef Fixed<1000000, long long> Fixed6;
typedef Fixed<192, unsigned short> FixedShort192;