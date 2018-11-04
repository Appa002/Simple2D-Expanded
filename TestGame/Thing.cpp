#include "Thing.hpp"
#include <Globals.h>
#include "ui.h"
#include <Simple2D.h>

int i = 0;
bool up = true;

void Thing::setup()
{
    findGameObject(this)->loadNewSprite("./square.png");

}

void Thing::update(long deltaTime)
{
    const uint8_t* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_Q])
        this->offset -= 0.01;
    if (state[SDL_SCANCODE_E])
        this->offset += 0.01;


    Ui::changeTextColour("", 255 - i, 255 + i, 255 - i);
    Simple2D::Ui::createText("Current FPS: " + std::to_string(1000 / deltaTime) + " hz", vec3(-0.95f, 0.8f, 0), vec3(0.02f, 0.02f, 0.02f));

    std::cout << 255 - i / 10 << std::endl;

    if(up) i++;
    else i--;

    if (i == 250 && up == true) {
        up = false;
    }
    else if (i == 0 && up == false) {
        up = true;
    }
}