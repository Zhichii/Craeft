#include "chunk.hpp"
#include "tools/log.hpp"
#include "component.hpp"
#include "tools/utf8.hpp"

int main() {
    log(LOG_INFO, "CraeftServer started.");
    std::string test = "千方百块";
    std::cout << test << std::endl;
    Utf8Wrapper wrapper(test);
    for (auto i : wrapper) {
        std::cout << int(i) << std::endl;
    }
    log(LOG_WARNING, "CraeftServer gave out a warning.");
    CronValue v { CronObject{} };
    std::get<CronObject>(v.v)["test"] = std::make_shared<CronValue>(5);
    std::get<CronObject>(v.v)["test2"] = std::make_shared<CronValue>(6);
    std::cout << v << std::endl;
    log(LOG_ERROR, "CraeftServer crashes!");
    int a = 0;
    int b = 5/a;
    return 0;
}