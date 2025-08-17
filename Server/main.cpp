#include "chunk.hpp"
#include "tools/log.hpp"
#include "../include/json/json.h"
#include "utf8.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    HWND hWnd = GetConsoleWindow();
    char buffer[129]; GetClassNameA(hWnd, buffer, 128);
    if (std::string(buffer) == "ConsoleWindowClass") {
        printf("Unsupported console. Please use Windows Terminal.");
        return;
    }
#endif
    log(LOG_INFO, "CraeftServer started.");
    std::string test = "千方百块";
    std::cout << test << std::endl;
    Utf8Wrapper wrapper(test);
    for (auto i : wrapper) {
        std::cout << i << std::endl;
    }
    log(LOG_WARNING, "CraeftServer gave out a warning.");
    Json::Value v;
    Json::Reader reader;
    reader.parse("{\"Kong rong\":\"Rang li\"}", v);
    std::cout << v << std::endl;
    log(LOG_ERROR, "CraeftServer crashed! 我故意崩的。");
    int a = 0;
    int b = 5/a;
    return 0;
}