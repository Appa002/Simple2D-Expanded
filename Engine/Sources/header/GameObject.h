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
        GLuint texture;
        bool hasTextureLoaded = false;
        bool markedForDeletion = false;

    public:
        GameObject();
        ~GameObject();

        Behavior* behavior;
        std::string name;
        int behaviorIdx;

        const GLuint& getTexture() const;
        const bool& shallRender() const;
        bool isMarkedForDeletion();
        
        void loadNewSprite(std::string path);
        void remove();
    };
}
#endif //SIMPLE2DENGINE_GAMEOBJECT_H
