#include "Camera.hpp"
#include "Object.hpp"
#include "MapManager.h"
#include <string>

void TestObj::init()
{
}

void TestObj::update(long deltaTime)
{
}

void TestObj::setup()
{
    findGameObject(this)->loadNewSprite("./square.png");
}

void TestObj::fixedUpdate()
{
    return;
    i++;
    auto o = MapManager::get()->getCurrentMap()->instanciatePrefab("test.prefabs.xml","Thing", std::to_string(i));
    auto ptr = o->behavior->viewAttribute<vec3>("position");
    ptr->x += 2 * i;
    
}

void TestObj::onRemoval()
{
}

void TestObj::onEvent(SDL_Event & e)
{
}
