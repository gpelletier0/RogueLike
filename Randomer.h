#pragma once

#include <random>

class Randomer {
private:
    Randomer() = default;

public:
    static int RandomInt(int min, int max);

};