#include "component.hpp"
#include <sstream>
#include <array>
#include <iomanip>
#include "utf8.hpp"

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
            Utf8Wrapper str(string);
            for (auto chr : str) {
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
                os << *item;
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
                os << "\"";
                Utf8Wrapper str(pair.first);
                for (auto chr : str) {
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
                os << "\"" << ": " << *pair.second;
            }
            os << "}";
        } break;
    }
    return os;
}

CronValue& CronValue::operator=(CronValue value) {
    v =value.v;
    return *this;
}

CronType CronValue::type() { return (CronType)v.index(); }

bool& CronValue::getBool() {
    if (v.index() != 1) throw std::runtime_error("Not boolean");
    return std::get<bool>(v);
}

int& CronValue::getInt() {
    if (v.index() != 2) throw std::runtime_error("Not int");
    return std::get<int>(v);
}

float& CronValue::getFloat() {
    if (v.index() != 3) throw std::runtime_error("Not float");
    return std::get<float>(v);
}

std::string& CronValue::getString() {
    if (v.index() != 4) throw std::runtime_error("Not string");
    return std::get<std::string>(v);
}

CronList& CronValue::getList() {
    if (v.index() != 5) throw std::runtime_error("Not list");
    return std::get<CronList>(v);
}

CronObject& CronValue::getObject() {
    if (v.index() != 6) throw std::runtime_error("Not object");
    return std::get<CronObject>(v);
}

CronValue& CronValue::operator[](std::string key) {
    if (v.index() == 6) {
        if (std::get<CronObject>(v).count(key) == 0) std::get<CronObject>(v)[key] = std::make_shared<CronValue>();
        return *std::get<CronObject>(v)[key];
    }
    else throw std::runtime_error("Not an object");
}

CronValue& CronValue::operator[](size_t idx) {
    if (v.index() == 5) {
        if (idx >= std::get<CronList>(v).size()) throw std::runtime_error("Out of bound");
        return *std::get<CronList>(v)[idx];
    }
    else throw std::runtime_error("Not an list");
}

void CronValue::append(CronValue o) {
    if (v.index() == 5) {
        std::get<CronList>(v).push_back(std::make_shared<CronValue>(o));
    }
    else throw std::runtime_error("Not an list");
}

