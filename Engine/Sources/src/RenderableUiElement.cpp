#include "..\header\RenderableUiElement.h"
#include <stb_image.h>
#include <iostream>


void Simple2D::RenderableUiElement::setupOpenglBuffer()
{
    glDeleteBuffers(1, posVbo);
    glDeleteBuffers(1, vtVbo);
    glDeleteBuffers(1, vao);

    glGenBuffers(1, posVbo);
    glGenBuffers(1, vtVbo);
    glGenVertexArrays(1, vao);

    GLfloat pos[]{
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, *posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

    GLfloat texcoords[]{
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, *vtVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);


    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *posVbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vtVbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);
}

Simple2D::RenderableUiElement::RenderableUiElement()
{
    vao = new GLuint(0);
    vtVbo = new GLuint(0);
    posVbo = new GLuint(0);
}


Simple2D::RenderableUiElement::~RenderableUiElement()
{
    glBindVertexArray(*vao);
    glDeleteBuffers(1, posVbo);
    glDeleteBuffers(1, vtVbo);
    glDeleteVertexArrays(1, vao);
    delete vao;
    delete vtVbo;
    delete posVbo;
}

void Simple2D::RenderableUiElement::render(GLuint shaderProgramme)
{
    glUseProgram(shaderProgramme);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.4);
    glEnable(GL_BLEND);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!loaded)
        return;
    {
        GLint loc = glGetUniformLocation(shaderProgramme, "pos");
        if (loc != -1) {
            float data[3];
            data[0] = pos.x;
            data[1] = pos.y;
            data[2] = pos.z;
            glUniform3fv(loc, 1, data);
        }
    }

    {
        GLint loc = glGetUniformLocation(shaderProgramme, "scale");
        if (loc != -1) {
            float data[3];
            data[0] = scale.x;
            data[1] = scale.y;
            data[2] = scale.z;
            glUniform3fv(loc, 1, data);
        }
    }

    GLint loc = glGetUniformLocation(shaderProgramme, "textColour");
    if (loc != -1) {
        float data[3];
        data[0] = activeColour.x / 255.0f;
        data[1] = activeColour.y / 255.0f;
        data[2] = activeColour.z / 255.0f;
        glUniform3fv(loc, 1, data);
    }

    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(*vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Simple2D::RenderableUiElement::loadImage(std::string path)
{
    //glGenVertexArrays(1, vao);
    //int n;
    //imageData = stbi_load(path.c_str(), imageWidth, imageHeight, &n, 4);

    //if (!imageData) {
    //    return;
    //}
    //this->bpp = 32;

    //// Check if dimensions are not a power of two.
    //// Older GPUs can't handel textures which are not a power of two.
    //if ((*imageWidth & (*imageWidth - 1)) != 0 || (*imageWidth & (*imageHeight - 1)) != 0) {
    //    printf("[WARNING] Dimensions not a power of two for ui element at: %p", this);
    //}

    //// Filp Images upside down
    //int widthInBytes = 4 * *imageWidth;
    //unsigned char* top = nullptr;
    //unsigned char* bottom = nullptr;
    //unsigned char tmp = 0;
    //int halfHeight = *imageHeight / 2;

    //for (int row = 0; row < halfHeight; row++) {
    //    top = imageData + row * widthInBytes;
    //    bottom = imageData + (*imageHeight - row - 1) * widthInBytes;
    //    for (int col = 0; col < widthInBytes; col++) {
    //        tmp = *top;
    //        *top = *bottom;
    //        *bottom = tmp;
    //        top++;
    //        bottom++;

    //    }
    //}
}

void Simple2D::RenderableUiElement::loadImageData(GLuint texture)
{
    setupOpenglBuffer();
    this->texture = texture;
    this->loaded = true;

}
