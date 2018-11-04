#pragma once

#include <string>
#include <vector>
#include <functional>

namespace {
    std::vector<std::string> tokenize(const std::string& str, const std::string& delimiters = " ")
    {
        std::vector<std::string> tokens;
        // Skip delimiters at beginning.
        std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
        // Find first "non-delimiter".
        std::string::size_type pos = str.find_first_of(delimiters, lastPos);

        while (std::string::npos != pos || std::string::npos != lastPos)
        {
            // Found a token, add it to the vector.
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            // Skip delimiters.  Note the "not_of"
            lastPos = str.find_first_not_of(delimiters, pos);
            // Find next "non-delimiter"
            pos = str.find_first_of(delimiters, lastPos);
        }
        return tokens;
    }
}

namespace Simple2D{
    using vec3 = 
        struct Vec3{
            float x,y,z;
            Vec3() = default;
            Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

            bool operator==(const Vec3& other) {
                return x == other.x && y == other.y && z == other.z;
            }

            bool operator!=(const Vec3& other) {
                return !(operator==(other));
            }
        };

    using vec2 =
        struct Vec2 {
            float x, y;
            Vec2() = default;
            Vec2(float x, float y) : x(x), y(y) {}

            bool operator==(const Vec2& other) {
                return x == other.x && y == other.y;
            }

            bool operator!=(const Vec2& other) {
                return !(operator==(other));
            }
        };

    using dvec3 =
        struct DVec3 {
            double x, y, z;
            DVec3() = default;
            DVec3(double x, double y, double z) : x(x), y(y), z(z) {}

            bool operator==(const DVec3& other) {
                return x == other.x && y == other.y && z == other.z;
            }

            bool operator!=(const DVec3& other) {
                return !(operator==(other));
            }
        };

    using dvec2 =
        struct DVec2 {
            double x, y;
            DVec2() = default;
            DVec2(double x, double y) : x(x), y(y) {}
            bool operator==(const DVec2& other) {
                return x == other.x && y == other.y;
            }

            bool operator!=(const DVec2& other) {
                return !(operator==(other));
            }
        };

   
    vec3 sto3(std::string s);

    vec2 sto2(std::string s);

    dvec3 stod3(std::string s);

    dvec2 stod2(std::string s);
}