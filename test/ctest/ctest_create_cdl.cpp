//
// Created by Howard Colin on 03/04/2023.
//

#include "../../src/cdl.h"
#include <iostream>

int createCDL() {
    float cdlArray[3][3] = {
            {0.11111, 0.11111, 0.11111},
            {0.22222, 0.22222, 0.22222},
            {0.11111, 0.11111, 0.11111}
    };
    colour::Cdl cdlData(&cdlArray, 0.55555);
    cdlData.printCDL();
    return 0;
}

int main() {
    return createCDL();
}