#pragma once
#include "time.hpp"
#include <iostream>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

template <typename... Ts>
void log(LogLevel level, Ts... args) {
    switch (level) {
        case LOG_INFO:
            break;
        case LOG_WARNING:
            std::cout << "\033[38;2;255;127;127m";
            break;
        case LOG_ERROR:
            std::cout << "\033[38;2;224;24;24m";
            break;
    }
    (std::cout << ... << args);
    std::cout << "\033[0m" << std::endl;
}