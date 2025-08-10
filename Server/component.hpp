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
class CronValue {
public:
    std::variant<std::monostate,bool,int,float,std::string,CronList,CronObject> v;
public:
    CronValue() { v = std::monostate{}; }
    template <typename T>
    CronValue(T t) { v = t; }
    friend std::ostream& operator<<(std::ostream& os, const CronValue& self);
};

class Component {
    std::map<std::string, CronValue> m;
};