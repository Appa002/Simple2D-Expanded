#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "singleton.h"
#include "GameObject.h"

namespace Simple2D {
    class GameObjectRenderer : public Singleton<GameObjectRenderer>
    {
    public:
        ~GameObjectRenderer();

    private:
        GLuint posVbo = 0;
        GLuint vtVbo = 0;
        GLuint vao = 0;
        GLuint shaderProgramme = 0;


    public:
        void render(const GameObject& gameObject);
        void setShaderProgramme(GLuint shaderProgramme);
        void setupOpenglBuffer();
        void setupShaders();

    };
}