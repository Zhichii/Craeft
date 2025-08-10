#include "component.hpp"

#include <sstream>
#include <array>
#include <iomanip>
#include "tools/utf8.hpp"

std::ostream& operator<<(std::ostream& os, const CronValue& self)  {
    switch (self.v.index()) {
        case 0: break;
        case 1: {
            switch (std::get<bool>(self.v)) {
                case true:
                    os << "true";
                    break;
                case false:
                    os << "false";
                    break;
            }
        } break;
        case 2: {
            os << std::get<int>(self.v);
        } break;
        case 3: {
            os << std::get<float>(self.v);
        } break;
        case 4: {
            auto& string = std::get<std::string>(self.v);
            os << "\"";
            for (unsigned char chr : string) {
                if (chr == '"') os << "\\\"";
                else if (chr == '\\') os << "\\\\";
                else if (chr == '\b') os << "\\b";
                else if (chr == '\f') os << "\\f";
                else if (chr == '\n') os << "\\n";
                else if (chr == '\r') os << "\\r";
                else if (chr == '\t') os << "\\t";
                else if (chr <= 0x1f) {
                    os << "\\u00";
                    std::ostringstream oss;
                    oss << std::setw(2) << std::setfill('0') << std::hex << (int)chr;
                    os << oss.str();
                }
                else os << chr;
            }
            os << "\"";
        } break;
        case 5: {
            auto& list = std::get<CronList>(self.v);
            os << "[";
            bool first = true;
            for (auto& item : list) {
                if (first) first = false;
                else os << ", ";
                os << item;
            }
            os << "]";
        } break;
        case 6: {
            auto& map = std::get<CronObject>(self.v);
            os << "{";
            bool first = true;
            for (auto& pair : map) {
                if (first) first = false;
                else os << ", ";
                os << pair.first << ": " << *pair.second;
            }
            os << "}";
        } break;
    }
    return os;
}