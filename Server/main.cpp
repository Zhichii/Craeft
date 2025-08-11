#include "chunk.hpp"
#include "tools/log.hpp"
#include "component.hpp"
#include "utf8.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    log(LOG_INFO, "CraeftServer started.");
    std::string test = "千方百块";
    std::cout << test << std::endl;
    Utf8Wrapper wrapper(test);
    for (auto i : wrapper) {
        std::cout << i << std::endl;
    }
    log(LOG_WARNING, "CraeftServer gave out a warning.");
    CronValue v { CronObject{} };
    v["test"] = 5;
    v["test2"] = CronList{};
    v["test2"].append("\n青色等烟雨，而我\x0b\"等你");
    std::cout << v << std::endl;
    v["test"] = "hi";
    std::cout << v << std::endl;
    log(LOG_ERROR, "CraeftServer crashed!");
    int a = 0;
    int b = 5/a;
    return 0;
}