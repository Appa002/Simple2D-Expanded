#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "types.h"
#include "RenderableUiElement.h"
#include <vector>
#include <map>
#include "singleton.h"
#include "Character.h"
#include "ExpandingMap.h"

namespace Simple2D {
    class UiManager : public Singleton<UiManager> {
    private:
        GLuint shaderProgramme;
        void createShader();

    public:
        std::vector<RenderableUiElement*> elements;
        std::unordered_map<std::string, ExpandingMap<char, Character>> fonts;
        void init();
        void renderAll();
        void loadFont();
        void loadBmpFont(std::string name, std::string path, size_t cellsPerRow, size_t cellsPerColumn,
            size_t cellWidth, size_t cellHeight);
    };
}