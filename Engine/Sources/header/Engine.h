//
// Created by albert on 1/3/18.
//

#ifndef SIMPLE2DENGINE_ENGINE_H
#define SIMPLE2DENGINE_ENGINE_H


#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <fstream>
#include "MapManager.h"
#include "UiManager.h"
#include "GameObjectRenderer.h"

namespace Simple2D{
    int destroyWindow();
    int createWindow(std::string name, int resX, int resY);
    int startMainLoop();
}



#endif //SIMPLE2DENGINE_ENGINE_H
