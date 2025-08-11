#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>
#include <memory>

class Component;

// CRON = Craeft Object Notation
class CronValue;
using CronList = std::vector<std::shared_ptr<CronValue>>;
using CronObject = std::map<std::string,std::shared_ptr<CronValue>>;
using CronTypes = std::variant<std::monostate,bool,int,float,std::string,CronList,CronObject>;
enum CronType { CRON_NULL, CRON_BOOL, CRON_INT, CRON_FLOAT, CRON_STRING, CRON_LIST, CRON_OBJECT };
class CronValue {
    CronTypes v;
public:
    CronValue() { v = std::monostate{}; }
    template <typename T>
    CronValue(T t) { v = t; }
    CronValue& operator=(CronValue value);
    CronType type();
    bool& getBool();
    int& getInt();
    float& getFloat();
    std::string& getString();
    CronList& getList();
    CronObject& getObject();
    CronValue& operator[](size_t idx); // For Lists
    void append(CronValue o); // For Lists
    CronValue& operator[](std::string key); // For Objects
    friend std::ostream& operator<<(std::ostream& os, const CronValue& self);
};

class Component {
    std::map<std::string, CronValue> m;
};
