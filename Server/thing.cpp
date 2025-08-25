#include "thing.hpp"
#include "utf8.hpp"
#include <sstream>

Identifier::Identifier(std::string name) {  
    bool state = 0;
    Utf8Wrapper utf8 {name};
    std::ostringstream oss1;
    std::ostringstream oss2;
    for (auto i : utf8) {
        if (i == ':') {
            if (state) throw std::runtime_error("invalid identifier");
            state = true;
            continue;
        }
        if (('0' <= i && i <= '9') || ('a' <= i && i <= 'z') || (i == '_')) {
            if (state == false) oss1 << toString(i);
            else oss2 << toString(i);
        }
        else throw std::runtime_error("invalid identifier");
    }
    if (state) { // 有“:” -> “craeft:stone”
        s = oss1.str();
        n = oss2.str();
    }
    else { // 没“:” -> “stone”
        s = "craeft";
        n = oss1.str();
    }
}

Identifier::Identifier(std::string namesp, std::string name) {
    Utf8Wrapper utf8s {namesp};
    for (auto i : utf8s) {
        if (('0' <= i && i <= '9') || ('a' <= i && i <= 'z') || (i == '_'));
        else throw std::runtime_error("invalid identifier");
    }
    s = namesp;
    Utf8Wrapper utf8n {name};
    for (auto i : utf8n) {
        if (('0' <= i && i <= '9') || ('a' <= i && i <= 'z') || (i == '_'));
        else throw std::runtime_error("invalid identifier");
    }
    n = name;
}

std::ostream& operator<<(std::ostream& os, const Identifier& identifier) {
    os << identifier.s << ":" << identifier.n;
    return os;
}

bool Identifier::operator<(const Identifier& other) const {
    if (s == other.s) return n < other.n;
    return s < other.s;
}

void to_language(Json::Value val, Language& lang) {
    if (val.type() != Json::objectValue) throw std::runtime_error("not an object");
    for (const std::string& key : val.getMemberNames()) {
        if (val[key].type() != Json::stringValue) throw std::runtime_error("not a valid language JSON");
        lang[key] = val[key].asString();
    }
}

Text::Translation::Translation(const std::string& key) { k = key; }

std::string Text::Translation::translate(Language& lang) {
    if (lang.count(k)) return lang[k];
    else return k;
}

Text& Text::operator[](std::string arg) { return args[arg]; }

Text::Text(): color(224,224,224), style(0) { }

Text Text::from_literal(const std::string& literal) {
    Text t; t.v = literal;
    return t;
}

Text Text::from_key(const std::string& literal) {
    Text t; t.v = Translation(literal);
    return t;
}

Text Text::from_list(const std::vector<Text>& list) {
    Text t; t.v = list;
    return t;
}

Text& Text::set_arg(const std::string& argument, Text value) {
    (*this)[argument] = value;
    return *this;
}

Text Text::with_arg(const std::string& argument, Text value) const {
    Text a = *this;
    return a.set_arg(argument, value);
}

Text& Text::set_bold(bool bold) {
    if (bold) style |= BOLD;
    else style &= ~BOLD;
    return *this;
}

Text& Text::set_italic(bool italic) {
    if (italic) style |= ITALIC;
    else style &= ~ITALIC;
    return *this;
}

Text& Text::set_underlined(bool underlined) {
    if (underlined) style |= UNDERLINED;
    else style &= ~UNDERLINED;
    return *this;
}

Text& Text::set_strikethrough(bool strikethrough) {
    if (strikethrough) style |= STRIKETHROUGH;
    else style &= ~STRIKETHROUGH;
    return *this;
}

Text& Text::set_color(Color clr) {
    color = clr;
    return *this;
}

Text Text::as_bold(bool bold) const {
    Text a = *this;
    return a.set_bold(bold);
}

Text Text::as_italic(bool italic) const {
    Text a = *this;
    return a.set_italic(italic);
}

Text Text::as_underlined(bool underlined) const {
    Text a = *this;
    return a.set_underlined(underlined);
}

Text Text::as_strikethrough(bool strikethrough) const {
    Text a = *this;
    return a.set_strikethrough(strikethrough);
}

Text Text::with_color(Color clr) const {
    Text a = *this;
    return a.set_color(clr);
}

bool Text::is_bold() const { return style & BOLD; }

bool Text::is_italic() const { return style & ITALIC; }

bool Text::is_underlined() const { return style & UNDERLINED; }

bool Text::is_strikethrough() const { return style & STRIKETHROUGH; }

Color Text::get_color() const { return color; }

std::string Text::to_string(Language& lang) {
    std::string output;
    if (v.index() == 0) output = std::get<std::string>(v);
    if (v.index() == 1) output = std::get<Translation>(v).translate(lang);
    if (v.index() == 2) {
        std::vector<Text>& vt = std::get<std::vector<Text>>(v);
        std::ostringstream oss;
        for (auto& text : vt) oss << text.to_string(lang);
        output = oss.str();
    }
    for (auto& k : args) {
        //output = output.replace(k.first, k.second.to_string());
    }
    return output;
}

std::string Text::to_formatted(Language& lang) {
    std::string format = "\033[";
    if (style & BOLD) format += "1;";
    if (style & ITALIC) format += "3;";
    if (style & UNDERLINED) format += "4;";
    if (style & STRIKETHROUGH) format += "9;";
    format += "38;2;"
     + std::to_string(color.r()*color.a()/256) + ";"
     + std::to_string(color.g()*color.a()/256) + ";"
     + std::to_string(color.b()*color.a()/256) + ";";
    if (format[format.size()-1] == ';') format[format.size()-1] = 'm';
    else format += "m";
    std::string str;
    if (v.index() == 0) str = std::get<std::string>(v);
    if (v.index() == 1) str = std::get<Translation>(v).translate(lang);
    if (v.index() == 2) {
        std::vector<Text>& vt = std::get<std::vector<Text>>(v);
        std::ostringstream oss;
        for (auto& text : vt) oss << text.to_formatted(lang);
        str = oss.str();
    }
    return format + str + "\033[0m";
}