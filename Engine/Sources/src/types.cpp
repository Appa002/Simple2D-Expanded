#include "../header/types.h"

using namespace Simple2D;

vec3 Simple2D::sto3(std::string s) {
    auto values = tokenize(s, ",");
    return vec3(std::stof(values.at(0)), std::stof(values.at(1)), std::stof(values.at(2)));
}

vec2 Simple2D::sto2(std::string s) {
    auto values = tokenize(s, ",");
    return vec2(std::stof(values.at(0)), std::stof(values.at(1)));
}

dvec3 Simple2D::stod3(std::string s) {
    auto values = tokenize(s, ",");
    return dvec3(std::stod(values.at(0)), std::stod(values.at(1)), std::stod(values.at(2)));
}

dvec2 Simple2D::stod2(std::string s) {
    auto values = tokenize(s, ",");
    return dvec2(std::stod(values.at(0)), std::stod(values.at(1)));
}
