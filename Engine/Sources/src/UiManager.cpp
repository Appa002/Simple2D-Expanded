#include "..\header\UiManager.h"
#include <stb_image.h>
#include <iostream>

void Simple2D::UiManager::init()
{
    createShader();
    loadFont();
    //loadBmpFont("Courier New", "./Courier_New.bmp", 16, 8, 32, 32);

  
}

void Simple2D::UiManager::renderAll()
{
    for (auto& element : elements) {
        element->render(shaderProgramme);
    }
    this->elements.clear();

}

void Simple2D::UiManager::loadFont()
{
    for (size_t i = 32; i <= 126; i++) {
        fonts["Arial"].get((int)i);
    }
}

void Simple2D::UiManager::loadBmpFont(std::string name, std::string path, size_t cellsPerRow, size_t cellsPerColumn, 
    size_t cellWidth, size_t cellHeight)
{
    int n;
    int imageWidth;
    int imageHeight;
    unsigned char* imageData;

    imageData = stbi_load(path.c_str(), &imageWidth, &imageHeight, &n, 4);

    if (!imageData) {
        printf("[ERROR] Can't load font file \"%s\"!", path.c_str());
        return;
    }

    // 512 by 256
    // 32 by 2
    for (size_t verticalCells = 0; verticalCells < cellsPerColumn; verticalCells++) {
        for (size_t horizontalCells = 0; horizontalCells < cellsPerRow; horizontalCells++) {
            unsigned char* data = new unsigned char[cellWidth * cellHeight * 4];

            for (size_t row = 0; row < cellHeight; row++) {
                size_t baseAddr = row * imageWidth * 4; // Calculate the position of the first pixel in each row ignoring offset.
                baseAddr += horizontalCells * cellWidth * 4 + verticalCells * imageWidth * 4 * cellHeight;

                memcpy(data + row * cellWidth * 4, imageData + baseAddr, cellWidth * 4);
            }
     //       fonts[name].push_back(data);
        }
    }


    //for (auto& letter : fonts[name]) {
    //    // Filp Images upside down
    //    int widthInBytes = 4 * cellWidth;
    //    unsigned char* top = nullptr;
    //    unsigned char* bottom = nullptr;
    //    unsigned char tmp = 0;
    //    int halfHeight = cellHeight / 2;

    //    for (int row = 0; row < halfHeight; row++) {
    //        top = letter + row * widthInBytes;
    //        bottom = letter + (32 - row - 1) * widthInBytes;
    //        for (int col = 0; col < widthInBytes; col++) {
    //            tmp = *top;
    //            *top = *bottom;
    //            *bottom = tmp;
    //            top++;
    //            bottom++;
    //        }
    //    }
    //}
}


void Simple2D::UiManager::createShader()
{
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    char* tmp = R"(
#version 410

layout (location=0) in vec3 vp;
layout (location=1) in vec2 vt;

uniform vec3 pos;
uniform vec3 scale;

out vec2 texture_coordinates;

void main(){
    vec3 finalPos;
    // Calculation of Local Position....
    finalPos.x = vp.x * scale.x;
    finalPos.y = vp.y * scale.y;
    finalPos.z = vp.z * scale.z;

    // Transformation into Screen Position....
    finalPos.x += pos.x;
    finalPos.y += pos.y;
    finalPos.z += pos.z;

    gl_Position = vec4(finalPos.x, finalPos.y, finalPos.z, 1.0f);
    texture_coordinates = vt;
})";
    glShaderSource(vs, 1, &tmp, NULL);
    glCompileShader(vs);

    GLint succses = 0;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &succses);
    if (!succses) {
        GLint logSize = 0;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &logSize);
        char* buffer = new char[logSize];
        glGetShaderInfoLog(vs, logSize, nullptr, buffer);
        std::cout << "Vertex Shader Compile Errors: " << "\n";
        std::cout << buffer << std::endl;
        delete buffer;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    char* tmp2 = R"(
#version 410

in vec2 texture_coordinates;
uniform sampler2D basic_texture;
uniform vec3 textColour;
out vec4 fragment_color;

void main(){
    vec4 texel = vec4(textColour.x, textColour.y, textColour.z, 
texture(basic_texture, texture_coordinates).r);
    fragment_color = texel;
}

)";
    glShaderSource(fs, 1, &tmp2, NULL);
    glCompileShader(fs);

    succses = 0;
    glGetShaderiv(fs, GL_COMPILE_STATUS, &succses);
    if (!succses) {
        GLint logSize = 0;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &logSize);
        char* buffer = new char[logSize];
        glGetShaderInfoLog(fs, logSize, nullptr, buffer);
        std::cout << "Fragment Shader Compile Errors: " << "\n";
        std::cout << buffer << std::endl;
        delete buffer;
    }

    shaderProgramme = glCreateProgram();
    glAttachShader(shaderProgramme, vs);
    glAttachShader(shaderProgramme, fs);
    glLinkProgram(shaderProgramme);

}