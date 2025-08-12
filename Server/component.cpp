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

CronValue::CronValue() { v = std::monostate{}; }

CronValue::CronValue(bool t) { v = t; }

CronValue::CronValue(const std::string& t) { v = t; }

CronValue::CronValue(const CronList& t) {
    v = CronList {};
    for (const auto& i : t) {
        std::get<CronList>(v).push_back(std::make_unique<CronValue>(*i));
    }
}

CronValue::CronValue(const CronObject& t) {
    v = CronObject {};
    for (const auto& i : t) {
        std::get<CronObject>(v)[i.first] = std::make_unique<CronValue>(*i.second);
    }
}

CronValue::CronValue(int t) { v = t; }

CronValue::CronValue(float t) { v = t; }

CronValue::CronValue(const char* t) { v = std::string(t); }

CronValue::CronValue(const CronTypes& t) {
    if (t.index() == 0);
    if (t.index() == 1) v = std::get<bool>(t);
    if (t.index() == 2) v = std::get<int>(t);
    if (t.index() == 3) v = std::get<float>(t);
    if (t.index() == 4) v = std::get<std::string>(t);
    if (t.index() == 5) {
        v = CronList {};
        for (const auto& i : std::get<CronList>(t)) {
            std::get<CronList>(v).push_back(std::make_unique<CronValue>(*i));
        }
    }
    if (t.index() == 6) {
        v = CronObject {};
        for (const auto& i : std::get<CronObject>(t)) {
            std::get<CronObject>(v)[i.first] = std::make_unique<CronValue>(*i.second);
        }
    }
}

CronValue::CronValue(const CronValue& other) {
    if (other.type() == 0);
    if (other.type() == 1) v = other.getBool();
    if (other.type() == 2) v = other.getInt();
    if (other.type() == 3) v = other.getFloat();
    if (other.type() == 4) v = other.getString();
    if (other.type() == 5) {
        v = CronList {};
        for (const auto& i : other.getList()) {
            std::get<CronList>(v).push_back(std::make_unique<CronValue>(*i));
        }
    }
    if (other.type() == 6) {
        v = CronObject {};
        for (const auto& i : other.getObject()) {
            std::get<CronObject>(v)[i.first] = std::make_unique<CronValue>(*i.second);
        }
    }
}

CronValue& CronValue::operator=(const CronValue& other) {
    if (other.type() == 0) v = std::monostate {};
    if (other.type() == 1) v = other.getBool();
    if (other.type() == 2) v = other.getInt();
    if (other.type() == 3) v = other.getFloat();
    if (other.type() == 4) v = other.getString();
    if (other.type() == 5) {
        v = CronList {};
        for (const auto& i : other.getList()) {
            std::get<CronList>(v).push_back(std::make_unique<CronValue>(*i));
        }
    }
    if (other.type() == 6) {
        v = CronObject {};
        for (const auto& i : other.getObject()) {
            std::get<CronObject>(v)[i.first] = std::make_unique<CronValue>(*i.second);
        }
    }
    return *this;
}

CronValue::CronValue(CronValue&& other) = default;

CronValue& CronValue::operator=(CronValue&& other) = default;

CronType CronValue::type() const { return (CronType)v.index(); }

bool& CronValue::getBool() {
    if (v.index() != 1) throw std::runtime_error("Not boolean");
    return std::get<bool>(v);
}
bool CronValue::getBool() const {
    if (v.index() != 1) throw std::runtime_error("Not boolean");
    return std::get<bool>(v);
}
int& CronValue::getInt() {
    if (v.index() != 2) throw std::runtime_error("Not int");
    return std::get<int>(v);
}
int CronValue::getInt() const {
    if (v.index() != 2) throw std::runtime_error("Not int");
    return std::get<int>(v);
}
float& CronValue::getFloat() {
    if (v.index() != 3) throw std::runtime_error("Not float");
    return std::get<float>(v);
}
float CronValue::getFloat() const {
    if (v.index() != 3) throw std::runtime_error("Not float");
    return std::get<float>(v);
}
std::string& CronValue::getString() {
    if (v.index() != 4) throw std::runtime_error("Not string");
    return std::get<std::string>(v);
}
const std::string& CronValue::getString() const {
    if (v.index() != 4) throw std::runtime_error("Not string");
    return std::get<std::string>(v);
}
CronList& CronValue::getList() {
    if (v.index() != 5) throw std::runtime_error("Not list");
    return std::get<CronList>(v);
}
const CronList& CronValue::getList() const {
    if (v.index() != 5) throw std::runtime_error("Not list");
    return std::get<CronList>(v);
}
CronObject& CronValue::getObject() {
    if (v.index() != 6) throw std::runtime_error("Not object");
    return std::get<CronObject>(v);
}
const CronObject &CronValue::getObject() const {
    if (v.index() != 6) throw std::runtime_error("Not object");
    return std::get<CronObject>(v);
}

CronValue& CronValue::operator[](std::string key) {
    if (v.index() == 6) {
        if (std::get<CronObject>(v).count(key) == 0) std::get<CronObject>(v)[key] = std::make_unique<CronValue>();
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
        std::get<CronList>(v).push_back(std::make_unique<CronValue>(o));
    }
    else throw std::runtime_error("Not an list");
}
