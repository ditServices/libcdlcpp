//
// Created by Howard Colin on 03/04/2023.
//
#include <filesystem>

#include "../../src/cdl.h"

namespace fs = std::filesystem;

int save_cdl(fs::path out) {
    float cdlArray[3][3] = {
            {0.11111, 0.11111, 0.11111},
            {0.22222, 0.22222, 0.22222},
            {0.11111, 0.11111, 0.11111}
    };

    colour::Cdl testCDL(&cdlArray, 0.654594);

    if(testCDL.saveCDL(out.c_str(), "Test"))
        return 0;
    else return 1;
}

int main() {
    return save_cdl("test.cdl");
}