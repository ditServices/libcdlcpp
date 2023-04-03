//
// Created by Howard Colin on 03/04/2023.
//

#ifndef CDL_CDL_H
#define CDL_CDL_H

#include "pugixml.hpp"

namespace colour {

    typedef struct Slope {
        float R, G, B;
    } Slope;

    typedef struct Offset {
        float R, G, B;
    } Offset;

    typedef struct Power {
        float R, G, B;
    } Power;

    class Cdl {
    private:
        Slope slope;
        Offset offset;
        Power power;
        float sat;

        inline void initCdlWriter();

        pugi::xml_document cdlFile;
        char * filePath;

    public:
        Cdl(Slope S, Offset O, Power P, float sat);
        int saveCDL(char * outputFilePath);
    };
}


#endif //CDL_CDL_H
