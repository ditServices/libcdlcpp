//
// Created by Howard Colin on 03/04/2023.
//

#ifndef CDL_CDL_H
#define CDL_CDL_H


#include "pugixml.hpp"

namespace colour {

    //custom float[3] types
    using Slope = float[3];
    using Offset = float[3];
    using Power = float[3];

    //helper print function
    void printSOP(float (&cdlVal)[3]);

    class Cdl {
    private:
        Slope mSlope;
        Offset mOffset;
        Power mPower;
        float mSat;
        std::string mId;
        pugi::xml_document mCdlFile;
    public:
        //constructors
        Cdl()
            : mSlope{0, 0, 0}, mOffset{0, 0, 0}, mPower{0, 0, 0}, mSat(0) {};

        Cdl(float(*cdl)[3][3], float sat); //from float[3][3] and sat

        explicit Cdl(const std::string_view &filePath); //load from file

        void setSOP(float (*s)[3], float (*o)[3], float (*p)[3]); //set via pointer
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
