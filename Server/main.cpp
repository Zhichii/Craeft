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
    Json::Reader reader;
    log(LOG_INFO, "CraeftServer started.");
    Json::Value v;
    reader.parse("{\"Craeft笑传之\":\"处处绊\"}", v);
    std::cout << v << std::endl;


    Language lang1;
    lang1["this.not"] = "这不是";
    lang1["literal"] = "里特劳（Literal）";
    lang1["dot"] = "。";
    Language lang2;
    lang2["this.not"] = "This is not ";
    lang2["literal"] = "Zimianliang（字面量）";
    lang2["dot"] = ". ";
    Text txt = Text::from_list({Text::from_key("this.not"), Text::from_key("literal").set_bold(), Text::from_key("dot")});
    std::cout << txt.to_formatted(lang1) << std::endl << txt.to_formatted(lang2) << std::endl;
    
    std::cout
     << Text::from_literal("黑").set_color(Color::BLACK).to_formatted(lang1)
     << Text::from_literal("红").set_color(Color::RED).to_formatted(lang1)
     << Text::from_literal("绿").set_color(Color::GREEN).to_formatted(lang1)
     << Text::from_literal("黄").set_color(Color::YELLOW).to_formatted(lang1)
     << Text::from_literal("蓝").set_color(Color::BLUE).to_formatted(lang1)
     << Text::from_literal("紫").set_color(Color::PURPLE).to_formatted(lang1)
     << Text::from_literal("青").set_color(Color::CYAN).to_formatted(lang1)
     << Text::from_literal("白").set_color(Color::WHITE).to_formatted(lang1)
     << Text::from_literal("114514").set_color("#114514").to_formatted(lang1)
     << Text::from_literal("229028").set_color("#229028").to_formatted(lang1)
     << Text::from_literal("fabric").set_color("#fab11c").to_formatted(lang1)
     << Text::from_literal("114514").set_color("#ffffff80").to_formatted(lang1)
     << std::endl;

    log(LOG_INFO, "CraeftServer started to register.");
    std::map<Identifier,Json::Value> registry;
    log(LOG_INFO, "CraeftServer finished registering.");
    log(LOG_ERROR, "CraeftServer crashed! 我故意的。");
    throw std::runtime_error("我故意崩的。");
    return 0;
}