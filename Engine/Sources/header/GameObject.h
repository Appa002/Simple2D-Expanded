//
// Created by albert on 1/3/18.
//


#ifndef SIMPLE2DENGINE_GAMEOBJECT_H
#define SIMPLE2DENGINE_GAMEOBJECT_H

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "types.h"
#include "Behavior.h"

namespace Simple2D{
    class  GameObject {
    private:
        int* spriteWidth;
        int* spriteHeight;
        unsigned char* imageData;
        bool markedForDeletion = false;
        GLuint texture;
        GLuint* posVbo;
        GLuint* vao;
        GLuint* vtVbo;

        void setupOpenglBuffer();
    public:
        GameObject();
        ~GameObject();

        Behavior* behavior;
        std::string name;
        int behaviorIdx;

        bool isMarkedForDeletion();
        void render(GLuint shaderProgramme);
        void loadNewSprite(std::string path);
        void remove();

    private:
        GameObject* findOtherGameObject(std::string name);
    };
}
#endif //SIMPLE2DENGINE_GAMEOBJECT_H
