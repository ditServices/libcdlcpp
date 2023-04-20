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
    for(int i = 0; i < 3; i++)
        std::cout << std::setprecision(6) << cdlVal[i] << " ";
    std::cout << std::endl;
}

//string conversion
template <typename T>
std::string ToString(T const & in_val)
{
    return std::to_string(in_val);
}

//constructor from [3][3] cdl matrix
colour::Cdl::Cdl(float (*cdl)[3][3], float sat) {

    //loop over each element and split into slope offset power arrays
    for(int s = 0; s < 3; s++) {
        this->slope[s] = (*cdl)[0][s];
    }

    for(int o = 0; o < 3; o++) {
        this->offset[o] = (*cdl)[1][o];
    }

    for(int p = 0; p < 3; p++) {
        this->power[p] = (*cdl)[2][p];
    }

    //saturation
    this->sat = sat;
}

//constructor load from file
colour::Cdl::Cdl(const char *filePath) {

    //load file and check for errors
    pugi::xml_parse_result result = this->cdlFile.load_file(filePath);
    if(!result) throw std::runtime_error("Error opening cdl file");

    try {
        //grab first node with attributes
        pugi::xml_node CDL = this->cdlFile.select_node("ColorDecisionList/ColorDecision/ColorCorrection").node();
        this->id = CDL.attribute("id").value(); // save this to CDL object
    } catch (const pugi::xpath_exception &e) {
        std::cout << "Select failed: " << e.what() << std::endl;
        throw std::runtime_error("Error whilst parsing CDL ID");
    }

    std::string token; //token for parsing.
    int i = 0; //counter

    try {
        //slope
        pugi::xml_node xmlSlope = this->cdlFile.select_node(
                "ColorDecisionList/ColorDecision/ColorCorrection/SOPNode/Slope/text()").node(); //grab the slope node
        //std::cout << xmlSlope.value() << std::endl;

        std::string sSlope = xmlSlope.value(); //grab the slope value into a string
        std::stringstream slopeStream(sSlope); //create a stream and point it at our string containing the values

        //iterate over the stream items by space.
        while (getline(slopeStream, token, ' ')) {
            this->slope[i++] = stof(token); //copy each slope item into each CDL object.
        }
        i = 0; //reset counter
    }
    catch (pugi::xpath_exception &e) {
        std::cout << "Select failed: " << e.what() << std::endl;
        throw std::runtime_error("Error whilst parsing Slope values");
    }

    try {
        //offset
        pugi::xml_node xmlOffset = this->cdlFile.select_node(
                "ColorDecisionList/ColorDecision/ColorCorrection/SOPNode/Offset/text()").node(); //grab the offset node
        //std::cout << xmlOffset.value() << std::endl;

        std::string sOffset = xmlOffset.value(); //grab the value into a string sOffset
        std::stringstream offsetStream(sOffset); //create a stream and point it at our string containing the values

        //iterate over each item by space.
        while (getline(offsetStream, token, ' ')) {
            this->offset[i++] = stof(token);
        }
        i = 0; //reset counter
    } catch (pugi::xpath_exception &e) {
        std::cout << "Select failed: " << e.what() << std::endl;
        throw std::runtime_error("Error whilst parsing Offset values");
    }
    try {
        //power
        pugi::xml_node xmlPower = this->cdlFile.select_node(
                "ColorDecisionList/ColorDecision/ColorCorrection/SOPNode/Power/text()").node(); //grab the power node
        //std::cout << xmlPower.value() << std::endl;

        std::string sPower = xmlPower.value(); //save the data to a string
        std::stringstream powerStream(sPower); //load the sPower string to a stream

        //iterate over each item by space
        while (getline(powerStream, token, ' ')) {
            this->power[i++] = stof(token);
        }
    } catch (pugi::xpath_exception &e) {
        std::cout << "Select failed: " << e.what() << std::endl;
        throw std::runtime_error("Error whilst parsing Power value");
    }

    try {
        //saturation
        pugi::xml_node xmlSat = this->cdlFile.select_node(
                "ColorDecisionList/ColorDecision/ColorCorrection/SATNode/Saturation/text()").node(); //grab the sat node
        std::string satString = xmlSat.value();
        this->sat = stof(satString);
    } catch (pugi::xpath_exception &e) {
        std::cout << "Select failed: " << e.what() << std::endl;
        throw std::runtime_error("Error whilst parsing Saturation value");
    }
}

//blank init
colour::Cdl::Cdl() {
    //default to zero
    this->sat = 0;
    for(int i=0; i < 3; i++) {
        this->slope[i] = 0;
        this->offset[i] = 0;
        this->power[i] = 0;
    }
}

//save CDL to file
bool colour::Cdl::saveCDL(const char * outputFilePath, const char *id) {
    //temp holding strings we need to build our xml data
    std::string slopeXml;
    std::string offsetXml;
    std::string powerXml;

    //create xml header and attributes
    pugi::xml_node decl = this->cdlFile.prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";

    //loop through each <float3> type and append to our holding strings.
    for(int i = 0; i < 3; i++) {
        slopeXml.append(ToString(this->slope[i]));
        slopeXml.append(" ");

        offsetXml.append(ToString(this->offset[i]));
        offsetXml.append(" ");

        powerXml.append(ToString(this->power[i]));
        powerXml.append(" ");
    }

    //create our root node
    pugi::xml_node root = this->cdlFile.append_child("ColorDecisionList");
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
    saturationNode.append_child(pugi::node_pcdata).set_value(ToString(this->sat).c_str());

    bool saveSucceeded = this->cdlFile.save_file(outputFilePath);
    if(saveSucceeded) {
        this->cdlFile.reset();
    }
    return saveSucceeded;
}

//pretty print the whole matrix
void colour::Cdl::printCDL() {

    std::cout << "ID: " << this->id << std::endl;

    //loop over slope
    for(int i = 0; i < 3; ++i) {
        std::cout << std::setprecision(6) << slope[i] << ", ";
    }
    std::cout << std::endl;

    //loop over offset
    for(int i = 0; i < 3; ++i) {
        std::cout << std::setprecision(6) << this->offset[i] << ", ";
    }

    std::cout << std::endl;

    //loop over power
    for(int i = 0; i < 3; ++i) {
        std::cout << std::setprecision(6) << this->power[i] << ", ";
    }
    std::cout << std::endl;

    //print saturation
    std::cout << "Saturation: " << this->sat << std::endl;
}

void colour::Cdl::getSlope(colour::Slope &slopeVal) {
    std::memcpy(slopeVal, this->slope, sizeof(this->slope));
}

void colour::Cdl::getOffset(colour::Offset &offsetVal) {
    std::memcpy(offsetVal, this->offset, sizeof(this->offset));
}

void colour::Cdl::getPower(colour::Power &powerVal) {
    std::memcpy(powerVal, this->power, sizeof(this->power));
}

float colour::Cdl::getSat() {
    return this->sat;
}

std::string colour::Cdl::getID() {
    return this->id;
}

void colour::Cdl::setSOP(float (*s)[3], float (*o)[3], float (*p)[3]) {

    for(int x = 0; x < 3; x++) {
        this->slope[x] = (*s)[x];
    }

    for(int x = 0; x < 3; x++) {
        this->offset[x] = (*o)[x];
    }

    for(int x = 0; x < 3; x++) {
        this->power[x] = (*p)[x];
    }

}

void colour::Cdl::setSat(float sat) {
    this->sat = sat;
}

void colour::Cdl::setID(std::string id) {
    this->id = std::move(id);
}




















