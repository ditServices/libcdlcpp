//
// Created by Howard Colin on 03/04/2023.
//

#include "cdl.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <utility>

//generic print helper function for float3 array
void colour::printSOP(float (&cdlVal)[3]) {
    for(auto value : cdlVal)
        std::cout << std::setprecision(6) << value << " ";
    std::cout << std::endl;
}

//string conversion
template <typename T>
std::string ToString(T const & in_val)
{
    return std::to_string(in_val);
}

//constructor from [3][3] cdl matrix
colour::Cdl::Cdl(float (*cdl)[3][3], float sat) : mSat(sat) {
    //loop over each element and split into slope offset power arrays
    for(int s = 0; s < 3; s++) {
        this->mSlope[s] = (*cdl)[0][s];
        this->mOffset[s] = (*cdl)[1][s];
        this->mPower[s] = (*cdl)[2][s];
    }
}

//constructor load from file
colour::Cdl::Cdl(const std::string_view &filePath) {

    //load file and check for errors
    pugi::xml_parse_result result = this->mCdlFile.load_file(filePath.data());
    if(!result) throw std::runtime_error("Error opening cdl file");

    try {
        //grab first node with attributes
        pugi::xml_node CDL = this->mCdlFile.select_node("ColorDecisionList/ColorDecision/ColorCorrection").node();
        this->mId = CDL.attribute("id").value(); // save this to CDL object
    } catch (const pugi::xpath_exception &e) {
        std::cout << "Select failed: " << e.what() << std::endl;
        throw std::runtime_error("Error whilst parsing CDL ID");
    }

    std::string token; //token for parsing.
    int i = 0; //counter

    try {
        //slope
        pugi::xml_node xmlSlope = this->mCdlFile.select_node(
                "ColorDecisionList/ColorDecision/ColorCorrection/SOPNode/Slope/text()").node(); //grab the slope node
        //std::cout << xmlSlope.value() << std::endl;

        std::string sSlope = xmlSlope.value(); //grab the slope value into a string
        std::stringstream slopeStream(sSlope); //create a stream and point it at our string containing the values

        //iterate over the stream items by space.
        while (getline(slopeStream, token, ' ')) {
            this->mSlope[i++] = stof(token); //copy each slope item into each CDL object.
        }
        i = 0; //reset counter
    }
    catch (pugi::xpath_exception &e) {
        std::cout << "Select failed: " << e.what() << std::endl;
        throw std::runtime_error("Error whilst parsing Slope values");
    }

    try {
        //offset
        pugi::xml_node xmlOffset = this->mCdlFile.select_node(
                "ColorDecisionList/ColorDecision/ColorCorrection/SOPNode/Offset/text()").node(); //grab the offset node

        std::string sOffset = xmlOffset.value(); //grab the value into a string sOffset
        std::stringstream offsetStream(sOffset); //create a stream and point it at our string containing the values

        //iterate over each item by space.
        while (getline(offsetStream, token, ' ')) {
            this->mOffset[i++] = stof(token);
        }
        i = 0; //reset counter
    } catch (pugi::xpath_exception &e) {
        std::cout << "Select failed: " << e.what() << std::endl;
        throw std::runtime_error("Error whilst parsing Offset values");
    }
    try {
        //power
        pugi::xml_node xmlPower = this->mCdlFile.select_node(
                "ColorDecisionList/ColorDecision/ColorCorrection/SOPNode/Power/text()").node(); //grab the power node

        std::string sPower = xmlPower.value(); //save the data to a string
        std::stringstream powerStream(sPower); //load the sPower string to a stream

        //iterate over each item by space
        while (getline(powerStream, token, ' ')) {
            this->mPower[i++] = stof(token);
        }
    } catch (pugi::xpath_exception &e) {
        std::cout << "Select failed: " << e.what() << std::endl;
        throw std::runtime_error("Error whilst parsing Power value");
    }

    try {
        //saturation
        pugi::xml_node xmlSat = this->mCdlFile.select_node(
                "ColorDecisionList/ColorDecision/ColorCorrection/SATNode/Saturation/text()").node(); //grab the sat node
        std::string satString = xmlSat.value();
        this->mSat = stof(satString);
    } catch (pugi::xpath_exception &e) {
        std::cout << "Select failed: " << e.what() << std::endl;
        throw std::runtime_error("Error whilst parsing Saturation value");
    }
}

//save CDL to file
bool colour::Cdl::saveCDL(const char * outputFilePath, const char *id) {
    //temp holding strings we need to build our xml data
    std::string slopeXml;
    std::string offsetXml;
    std::string powerXml;

    //create xml header and attributes
    pugi::xml_node decl = this->mCdlFile.prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";

    //loop through each <float3> type and append to our holding strings.
    for(int i = 0; i < 3; i++) {
        slopeXml.append(ToString(this->mSlope[i]));
        slopeXml.append(" ");

        offsetXml.append(ToString(this->mOffset[i]));
        offsetXml.append(" ");

        powerXml.append(ToString(this->mPower[i]));
        powerXml.append(" ");
    }

    //create our root node
    pugi::xml_node root = this->mCdlFile.append_child("ColorDecisionList");
    root.append_attribute("xmlns") = "urn:ASC:CDL:v1.2";

    //create some of the child nodes before main data is added
    pugi::xml_node colorDecision = root.append_child("ColorDecision");
    pugi::xml_node colorCorrection = colorDecision.append_child("ColorCorrection");
    colorCorrection.append_attribute("id") = id; //id attribute for ColorCorrection node as seen in <const char *id> func dec

    pugi::xml_node sopNode = colorCorrection.append_child("SOPNode"); //main node that houses slope offset power child nodes

    //Slope section
    pugi::xml_node slopeNode = sopNode.append_child("Slope");
    slopeNode.append_child(pugi::node_pcdata).set_value(slopeXml.c_str()); //add plain char data <pugi::node_pcdata>

    //Offset section
    pugi::xml_node offsetNode = sopNode.append_child("Offset");
    offsetNode.append_child(pugi::node_pcdata).set_value(offsetXml.c_str()); //add data

    //Power section
    pugi::xml_node powerNode = sopNode.append_child("Power");
    powerNode.append_child(pugi::node_pcdata).set_value(powerXml.c_str()); //add data

    //Saturation section.
    pugi::xml_node satNode = colorCorrection.append_child("SATNode");
    pugi::xml_node saturationNode = satNode.append_child("Saturation");
    saturationNode.append_child(pugi::node_pcdata).set_value(ToString(this->mSat).c_str());

    if(!this->mCdlFile.save_file(outputFilePath)) {
        return false;
    }

    this->mCdlFile.reset(); //reset our document so that we don't append to it.
    return true;
}

//pretty print the whole matrix
void colour::Cdl::printCDL() {

    std::cout << "ID: " << this->mId << std::endl;

    //loop over slope
    for(int i = 0; i < 3; ++i) {
        std::cout << std::setprecision(6) << mSlope[i] << ", ";
    }
    std::cout << std::endl;

    //loop over offset
    for(int i = 0; i < 3; ++i) {
        std::cout << std::setprecision(6) << this->mOffset[i] << ", ";
    }

    std::cout << std::endl;

    //loop over power
    for(int i = 0; i < 3; ++i) {
        std::cout << std::setprecision(6) << this->mPower[i] << ", ";
    }
    std::cout << std::endl;

    //print saturation
    std::cout << "Saturation: " << this->mSat << std::endl;
}

void colour::Cdl::getSlope(colour::Slope &slopeVal) {
    std::memcpy(slopeVal, this->mSlope, sizeof(this->mSlope));
}

void colour::Cdl::getOffset(colour::Offset &offsetVal) {
    std::memcpy(offsetVal, this->mOffset, sizeof(this->mOffset));
}

void colour::Cdl::getPower(colour::Power &powerVal) {
    std::memcpy(powerVal, this->mPower, sizeof(this->mPower));
}

float colour::Cdl::getSat() {
    return this->mSat;
}

std::string colour::Cdl::getID() {
    return this->mId;
}

void colour::Cdl::setSOP(float (*s)[3], float (*o)[3], float (*p)[3]) {

    for( int index = 0; index < 3; index++) {
        this->mSlope[index] = (*s)[index];
        this->mOffset[index] = (*o)[index];
        this->mPower[index] = (*p)[index];
    }

}

void colour::Cdl::setSat(float sat) {
    this->mSat = sat;
}

void colour::Cdl::setID(std::string id) {
    this->mId = std::move(id);
}




















