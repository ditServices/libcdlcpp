//
// Created by Howard Colin on 03/04/2023.
//

#include "cdl.h"

void colour::Cdl::initCdlWriter() {
    auto declarationNode = this->cdlFile.append_child(pugi::node_declaration);
    declarationNode.attribute("version") = 1;
    declarationNode.attribute("encoding") = "UTF-8";
}

int colour::Cdl::saveCDL(char * outputFilePath) {
    initCdlWriter();
    return 0;
}

colour::Cdl::Cdl(colour::Slope S, colour::Offset O, colour::Power P, float sat) {
    this->slope = S;
    this->offset = O;
    this->power = P;
    this->sat = sat;
}








