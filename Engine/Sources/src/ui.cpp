#include "../header/ui.h"

#include "../header/UiManager.h"
#include <iostream>
#include <stb_image.h>
#include <cstring>

using namespace Simple2D;

vec3 activeColour = vec3(255, 255, 255);

void Simple2D::Ui::createText(std::string text, vec3 pos, vec3 scale, float characterGap)
{
    float penX = pos.x;
    float penY = pos.y;
    float onePixelWidth = 2.0f / 1024.0f;

    for (auto c : text) {
        const auto& glyph = UiManager::get()->fonts["Arial"].get(c);
        penY = pos.y + glyph.advance.y * onePixelWidth * scale.y / 64.0f + scale.y * 2;

        auto element = new RenderableUiElement();

        element->pos = vec3(penX + scale.x + glyph.bearing.x * onePixelWidth * scale.x, 
            penY + scale.y + glyph.bearing.y * onePixelWidth * scale.y,
            pos.z);


        element->scale = vec3(scale.x * (glyph.size.x / glyph.fontSize),
            scale.y * (glyph.size.y / glyph.fontSize), 
            0);

        element->loadImageData(glyph.texture);
        element->activeColour = activeColour;
        UiManager::get()->elements.push_back(element);
         
        
        penX += glyph.advance.x * onePixelWidth * scale.x / 64.0f + scale.x * characterGap;
    }
}

void Simple2D::Ui::changeTextColour(std::string fontName, unsigned char red, unsigned char green, unsigned char blue)
{
    activeColour.x = red;
    activeColour.y = green;
    activeColour.z = blue;
}
