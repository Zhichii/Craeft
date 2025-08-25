#include "chunk.hpp"
#include "tools/log.hpp"
#include "tools/fixed.hpp"
#include "../include/json/json.h"
#include "thing.hpp"

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
        return 0;
    }
#endif
    log(LOG_INFO, "CraeftServer started.");
    Json::Value v;
    Json::Reader reader;
    reader.parse("{\"Craeft笑传之\":\"处处绊\"}", v);
    std::cout << v << std::endl;
    
    log(LOG_INFO, "CraeftServer started to register.");
    std::map<Identifier,Json::Value> registry;
    log(LOG_INFO, "CraeftServer finished registering.");
    log(LOG_ERROR, "CraeftServer crashed! 我故意的。");
    throw std::runtime_error("我故意崩的。");
    return 0;
}