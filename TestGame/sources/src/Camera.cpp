#include "Camera.hpp"

void Camera::init()
{
    position = viewAttribute<vec3>("position");
    scale = viewAttribute<vec3>("scale");
}

void Camera::update(long deltaTime)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_W]) // w 119
        position->y += 0.005f * (1 / scale->y) * deltaTime;
    if (state[SDL_SCANCODE_S]) // s 115
        position->y -= 0.005f * (1 / scale->y) * deltaTime;
    if (state[SDL_SCANCODE_A]) // a 97
        position->x -= 0.005f * (1 / scale->x) * deltaTime;
    if (state[SDL_SCANCODE_D]) // d 100
        position->x += 0.005f * (1 / scale->x) *deltaTime;

    if (state[SDL_SCANCODE_UP]) {
        scale->x += 0.001f * deltaTime;
        scale->y += 0.001f * deltaTime;
        scale->z += 0.001f * deltaTime;
    }

    if (state[SDL_SCANCODE_DOWN]) {
        scale->x -= 0.001f * deltaTime;
        scale->y -= 0.001f * deltaTime;
        scale->z -= 0.001f * deltaTime;
    }


}

void Camera::setup()
{
}

void Camera::onRemoval()
{
}

void Camera::onEvent(SDL_Event & e)
{}
