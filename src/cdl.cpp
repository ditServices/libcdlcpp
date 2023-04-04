//
// Created by Howard Colin on 03/04/2023.
//

#include "cdl.h"
#include <iostream>
#include <iomanip>
#include <sstream>

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
    pugi::xml_parse_result result = this->cdlFile.load_file(filePath, pugi::parse_default|pugi::parse_declaration);

    std::string xmlVal = this->cdlFile.child_value("Slope");
    std::stringstream inSlopeVal(xmlVal);

    std::string token;
    int i = 0;

    while (getline(inSlopeVal, token, ' ')) {
        this->slope[i++] = stof(token);
    }

    xmlVal = this->cdlFile.child("Offset").value();
    std::stringstream inOffsetVal(xmlVal);
    i = 0;

    while(getline(inOffsetVal, token, ' ')) {
        this->offset[i++] = stof(token);
    }

    xmlVal = this->cdlFile.child("Power").value();
    std::stringstream inPowerVal(xmlVal);
    i = 0;

    while(getline(inPowerVal, token, ' ')) {
        this->power[i++] = stof(token);
    }

//    xmlVal = this->cdlFile.child("Saturation").value();
//    this->sat = stof(xmlVal);
}


void colour::Cdl::initCdlWriter() {
    pugi::xml_node decl = this->cdlFile.prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";
}

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
}













