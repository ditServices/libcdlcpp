//
// Created by Howard Colin on 05/04/2023.
//

#include <iostream>

#include "../../src/cdl.h"

int test_getters() {
    try {
        colour::Cdl CDL("test.cdl");
        colour::Slope slopeVal;
        colour::Offset offsetVal;
        colour::Power powerVal;

        std::cout << "ID: " << CDL.getID() << std::endl;

        CDL.getSlope(slopeVal);
        colour::printSOP(slopeVal);

        CDL.getOffset(offsetVal);
        colour::printSOP(offsetVal);

        CDL.getPower(powerVal);
        colour::printSOP(powerVal);

        std::cout << "Sat: " << CDL.getSat() << std::endl;

    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

int main() {
    return test_getters();
}