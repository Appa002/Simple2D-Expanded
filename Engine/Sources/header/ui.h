#pragma once

#include "types.h"

namespace Simple2D{
    
    namespace Ui {
        void createText(std::string text, vec3 pos, vec3 scale = vec3(0.1, 0.1, 0.1), float characterGap = 1.5f);
        void changeTextColour(std::string fontName, unsigned char red, unsigned char green, unsigned char blue);
    }
}

