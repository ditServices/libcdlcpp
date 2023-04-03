//
// Created by Howard Colin on 03/04/2023.
//

#include "../../src/cdl.h"
#include <iostream>

int createCDL(colour::Slope S, colour::Offset O, colour::Power P, float sat) {
    colour::Cdl cdl(S, O, P, sat);
    return 0;
}

int main() {
    colour::Slope S {1, 1, 1};
    colour::Offset O {0, 0 ,0};
    colour::Power P {1, 1, 1};

    std::cout << S.R << S.G << S.B << std::endl;
    std::cout << O.R << O.G << O.B << std::endl;
    std::cout << P.R << P.G << P.B << std::endl;

    return createCDL(S, O, P, 1);
}