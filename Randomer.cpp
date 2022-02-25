//
// Created by gpelletier on 2022-02.
//

#include "Randomer.h"

/**
 * Generates a random integer between two values
 * @param min minimum value
 * @param max maximum value
 * @return a random integer between min and max
 */
int Randomer::RandomInt(const int min, const int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);

    return dis(gen);
}