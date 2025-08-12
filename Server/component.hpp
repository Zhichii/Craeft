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
using CronList = std::vector<std::unique_ptr<CronValue>>;
using CronObject = std::map<std::string,std::unique_ptr<CronValue>>;
using CronTypes = std::variant<std::monostate,bool,int,float,std::string,CronList,CronObject>;
enum CronType { CRON_NULL, CRON_BOOL, CRON_INT, CRON_FLOAT, CRON_STRING, CRON_LIST, CRON_OBJECT };
class CronValue {
    CronTypes v;
public:
    CronValue();
    CronValue(const char* t);
    CronValue(const std::string& t);
    CronValue(const CronList& t);
    CronValue(const CronObject& t);
    CronValue(int t);
    CronValue(float t);
    explicit CronValue(bool t);
    CronValue(const CronTypes& other);
    CronValue(const CronValue& other);
    CronValue& operator=(const CronValue& other);
    CronValue(CronValue&& other);
    CronValue& operator=(CronValue&& other);
    CronType type() const;
    bool& getBool();
    bool getBool() const;
    int& getInt();
    int getInt() const;
    float& getFloat();
    float getFloat() const;
    std::string& getString();
    const std::string& getString() const;
    CronList& getList();
    const CronList& getList() const;
    CronObject& getObject();
    const CronObject& getObject() const;
    CronValue& operator[](size_t idx); // For Lists
    void append(CronValue o); // For Lists
    CronValue& operator[](std::string key); // For Objects
    friend std::ostream& operator<<(std::ostream& os, const CronValue& self);
};

class Component {
    std::map<std::string, CronValue> m;
};
