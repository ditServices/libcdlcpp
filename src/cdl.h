//
// Created by Howard Colin on 03/04/2023.
//

#ifndef CDL_CDL_H
#define CDL_CDL_H

#include "pugixml.hpp"

namespace colour {

    typedef float Slope[3];
    typedef float Offset[3];
    typedef float Power[3];

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
        Cdl(float(*cdl)[3][3], float sat);
        Cdl(const char *filePath);
        void printCDL();
        bool saveCDL(const char * outputFilePath, const char *id);
    };
}


#endif //CDL_CDL_H
