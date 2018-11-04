#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "types.h"

namespace Simple2D {
    struct Character
    {
        GLuint texture; // ID of the texture
        vec2 bearing; // Offset from the base line
        vec2 size; // Size of the glyph
        vec2 advance; // Offset to next character.
        unsigned int fontSize; // What was specefied to be the size of the font. (Used to calculate the relative size of each character).

        Character() = default;
        Character(char c);
    };
}