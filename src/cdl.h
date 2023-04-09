//
// Created by Howard Colin on 03/04/2023.
//

#ifndef CDL_CDL_H
#define CDL_CDL_H


#include "pugixml.hpp"

namespace colour {

    //custom float[3] types
    typedef float Slope[3];
    typedef float Offset[3];
    typedef float Power[3];

    //helper print function
    void printSOP(float (&cdlVal)[3]);

    class Cdl {
    private:
        Slope slope;
        Offset offset;
        Power power;
        float sat;
        std::string id;
        pugi::xml_document cdlFile;
        char * filePath;
    public:
        //constructors
        Cdl();
        Cdl(float(*cdl)[3][3], float sat);
        explicit Cdl(const char *filePath);

        void setSOP(float (*s)[3], float (*o)[3], float (*p)[3]);
        void setSat(float sat);
        void setID(std::string id);

        void printCDL();
        void getSlope(Slope &slopeVal);
        void getOffset(Offset &offsetVal);
        void getPower(Power &powerVal);
        float getSat();
        std::string getID();
        bool saveCDL(const char * outputFilePath, const char *id);
    };
}

#endif //CDL_CDL_H
