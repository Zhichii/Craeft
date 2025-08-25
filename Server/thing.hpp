#pragma once

#include <string>
#include <ostream>
#include <map>
#include <variant>
#include <vector>
#include "../include/json/json.h"
#include "color.hpp"

class Identifier {
    std::string s;
    std::string n;
public:
    // Both `craeft:stone` and `stone` are supported.
    // May error.
    Identifier(std::string name);
    Identifier(std::string namesp, std::string name);
    friend std::ostream& operator<<(std::ostream& os, const Identifier& identifier);
    bool operator<(const Identifier& other) const;
};

typedef std::map<std::string, std::string> Language;

void to_language(Json::Value val, Language& lang);

class Text {
public:
    class Translation {
        std::string k;
    public:
        Translation(const std::string& key);
        std::string translate(Language& lang);
    };
private:
    std::variant<std::string, Translation, std::vector<Text>> v;
    std::map<std::string, Text> args;
    Text& operator[](std::string arg);
    unsigned short style;
    Color color;
public:
    const static unsigned short BOLD = 0b1;
    const static unsigned short ITALIC = 0b10;
    const static unsigned short UNDERLINED = 0b100;
    const static unsigned short STRIKETHROUGH = 0b1000;
    Text();
    static Text from_literal(const std::string& literal);
    static Text from_key(const std::string& key);
    // 请注意，若使用from_list，则其中每个片段是独立的，不会互相影响，也不会叠加。
    static Text from_list(const std::vector<Text>& list);
    // 试用一下方法链
    Text& set_arg(const std::string& argument, Text value);
    Text with_arg(const std::string& argument, Text value) const;
    Text& set_bold(bool bold = true);
    Text& set_italic(bool italic = true);
    Text& set_underlined(bool underlined = true);
    Text& set_strikethrough(bool strikethrough = true);
    Text& set_color(Color color);
    Text as_bold(bool bold = true) const;
    Text as_italic(bool italic = true) const;
    Text as_underlined(bool underlined = true) const;
    Text as_strikethrough(bool strikethrough = true) const;
    Text with_color(Color color) const;
    bool is_bold() const;
    bool is_italic() const;
    bool is_underlined() const;
    bool is_strikethrough() const;
    Color get_color() const;
    std::string to_string(Language& lang);
    std::string to_formatted(Language& lang);
};