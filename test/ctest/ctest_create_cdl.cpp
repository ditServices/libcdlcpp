//
// Created by Howard Colin on 03/04/2023.
//

#include "../../src/cdl.h"
#include <iostream>

int createCDL() {
    float cdlArray[3][3] = {
            {2.071965, 2.072090, 2.071920},
            {-0.132113, -0.132118, -0.132085},
            {1.598900, 1.598890, 1.598893}
    };
    colour::Cdl cdlData(&cdlArray, 1);
    cdlData.printCDL();
    return 0;
}

int main() {
    return createCDL();
}