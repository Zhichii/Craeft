#include "chunk.hpp"
#include "tools/log.hpp"

int main() {
    log(LOG_INFO, "CraeftServer started.");
    log(LOG_WARNING, "CraeftServer gave out a warning.");
    log(LOG_ERROR, "CraeftServer crashes!");
    int a = 0;
    int b = 5/a;
    return 0;
}