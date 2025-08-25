#pragma once

#include <map>
#include <string>
#include "../include/json/json.h"
#include "thing.hpp"

template <class T>
using Registry = std::map<Identifier,T>;
typedef Registry<Json::Value> ThingRegistry;

class Mod {
    
public:
    Mod(Json::Value val) {

    }
};