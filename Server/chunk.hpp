#pragma once

#include "thing.hpp"
#include "tools/fixed.hpp"

class Chunk {
    const static int WIDTH = 24;
public:
    Chunk();
};

class PositionI {
    Chunk* chunk;
    FixedShort192 x;
    FixedShort192 y;
    FixedShort192 z;
public:
    PositionI(long long x, long long y, long long z);
};
