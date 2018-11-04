#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "types.h"

namespace Simple2D {
    class RenderableUiElement
    {
    public:
        vec3 pos;
        vec3 scale;
        GLuint* vao;
        GLuint texture;
        bool loaded = false;
        vec3 activeColour = vec3(255, 255, 255);

    public:
        RenderableUiElement();
        ~RenderableUiElement();

        void render(GLuint shaderProgramme);
        void loadImage(std::string path);
        void loadImageData(GLuint texture);
    };
}
