//
// Created by Howard Colin on 03/04/2023.
//

#include "cdl.h"
#include <iostream>
#include <iomanip>
#include <sstream>

template <typename T>
std::string ToString(T const & in_val)
{
    return std::to_string(in_val);
}
// Specialization for boolean type to force "true"/"false"
template<>
std::string ToString(bool const & in_val)
{
    std::ostringstream oss;
    oss << std::boolalpha << in_val;
    return oss.str();
}

colour::Cdl::Cdl(float (*cdl)[3][3], float sat) {
    for(int s = 0; s < 3; s++) {
        this->slope[s] = (*cdl)[0][s];
    }

    for(int o = 0; o < 3; o++) {
        this->offset[o] = (*cdl)[1][o];
    }

    for(int p = 0; p < 3; p++) {
        this->power[p] = (*cdl)[2][p];
    }

    this->sat = sat;
}

colour::Cdl::Cdl(const char *filePath) {
    //load file and check for errors
    pugi::xml_parse_result result = this->cdlFile.load_file(filePath);
    if(!result) this->status = 1;

    //grab first node with attributes
    pugi::xml_node CDL = this->cdlFile.select_node("ColorDecisionList/ColorDecision/ColorCorrection").node();
    this->id = CDL.attribute("id").value(); // save this to CDL object

    std::string token; //token for parsing.
    int i = 0; //counter

    //slope
    pugi::xml_node xmlSlope = this->cdlFile.select_node("ColorDecisionList/ColorDecision/ColorCorrection/SOPNode/Slope/text()").node(); //grab the slope node
    //std::cout << xmlSlope.value() << std::endl;

    std::string sSlope = xmlSlope.value(); //grab the slope value into a string
    std::stringstream slopeStream(sSlope); //create a stream and point it at our string containing the values

    //iterate over the stream items by space.
    while(getline(slopeStream, token, ' ')) {
        this->slope[i++] = stof(token); //copy each slope item into each CDL object.
    }
    i = 0; //reset counter

    //offset
    pugi::xml_node xmlOffset = this->cdlFile.select_node("ColorDecisionList/ColorDecision/ColorCorrection/SOPNode/Offset/text()").node(); //grab the offset node
    //std::cout << xmlOffset.value() << std::endl;

    std::string sOffset = xmlOffset.value(); //grab the value into a string sOffset
    std::stringstream offsetStream(sOffset); //create a stream and point it at our string containing the values

    //iterate over each item by space.
    while(getline(offsetStream, token, ' ')) {
        this->offset[i++] = stof(token);
    }
    i = 0; //reset counter

    //power
    pugi::xml_node xmlPower = this->cdlFile.select_node("ColorDecisionList/ColorDecision/ColorCorrection/SOPNode/Power/text()").node(); //grab the power node
    //std::cout << xmlPower.value() << std::endl;

    std::string sPower = xmlPower.value(); //save the data to a string
    std::stringstream powerStream(sPower); //load the sPower string to a stream

    //iterate over each item by space
    while(getline(powerStream, token, ' ')) {
        this->power[i++] = stof(token);
    }

    //saturation
    pugi::xml_node xmlSat = this->cdlFile.select_node("ColorDecisionList/ColorDecision/ColorCorrection/SATNode/Saturation/text()").node(); //grab the sat node
    std::string satString = xmlSat.value();
    this->sat = stof(satString);

}


void colour::Cdl::initCdlWriter() {
    pugi::xml_node decl = this->cdlFile.prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";
}

bool colour::Cdl::saveCDL(const char * outputFilePath, const char *id) {
    initCdlWriter();

    std::string slopeXml;
    std::string offsetXml;
    std::string powerXml;

    for(int i = 0; i < 3; i++) {
        slopeXml.append(ToString(this->slope[i]));
        slopeXml.append(" ");

        offsetXml.append(ToString(this->offset[i]));
        offsetXml.append(" ");

        powerXml.append(ToString(this->power[i]));
        powerXml.append(" ");
    }

    auto root = this->cdlFile.append_child("ColorDecisionList");
    root.append_attribute("xmlns") = "urn:ASC:CDL:v1.2";
    auto colorDecision = root.append_child("ColorDecision");
    auto colorCorrection = colorDecision.append_child("ColorCorrection");
    colorCorrection.append_attribute("id") = id;
    auto sopNode = colorCorrection.append_child("SOPNode");

    auto slopeNode = sopNode.append_child("Slope");
    slopeNode.append_child(pugi::node_pcdata).set_value(slopeXml.c_str());

    auto offsetNode = sopNode.append_child("Offset");
    offsetNode.append_child(pugi::node_pcdata).set_value(offsetXml.c_str());

    auto powerNode = sopNode.append_child("Power");
    powerNode.append_child(pugi::node_pcdata).set_value(powerXml.c_str());

    auto satNode = colorCorrection.append_child("SATNode");

    auto saturationNode = satNode.append_child("Saturation");
    saturationNode.append_child(pugi::node_pcdata).set_value(ToString(this->sat).c_str());

    bool saveSucceeded = this->cdlFile.save_file(outputFilePath);
    return saveSucceeded;
}

void colour::Cdl::printCDL() {

    std::cout << "ID: " << this->id << std::endl;

    for(int i = 0; i < 3; ++i) {
        std::cout << std::setprecision(6) << slope[i] << ", ";
    }

    std::cout << std::endl;

    for(int i = 0; i < 3; ++i) {
        std::cout << std::setprecision(6) << this->offset[i] << ", ";
    }

    std::cout << std::endl;

    for(int i = 0; i < 3; ++i) {
        std::cout << std::setprecision(6) << this->power[i] << ", ";
    }
    std::cout << std::endl;

    std::cout << "Saturation: " << this->sat << std::endl;
}













