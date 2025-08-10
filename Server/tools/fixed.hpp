#pragma once

#include <iostream>

// 定点数。
template <int SCALE>
class Fixed {
private:
    long long value;
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
        result.value = this->value * that.value / SCALE;
        return result;
    }
    Fixed& operator*=(Fixed that) {
        this->value *= that.value;
        this->value /= SCALE;
        return *this;
    }
    Fixed operator/(Fixed that) {
        Fixed result;
        result.value = this->value * SCALE / that.value;
        return result;
    }
    Fixed& operator/=(Fixed that) {
        this->value *= SCALE;
        this->value /= that.value;
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

typedef Fixed<1000000> Fixed6;
typedef Fixed<1048576> FixedBin20;