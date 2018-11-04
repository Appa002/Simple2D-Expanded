#include "../header/Simple2D.h"
#include "../header/types.h"
#include "../header/MapManager.h"
#include <sstream>
#include <fstream>
#include <rapidxml.hpp>

Simple2D::GameObject* Simple2D::findGameObject(std::string name) {
    for (auto gObj : *MapManager::get()->getCurrentMap()->gameObjects) {
        if (gObj->name == name) {
            return gObj;
        }
    }
    return nullptr;
}

Simple2D::GameObject *Simple2D::findGameObject(Simple2D::Behavior *address) {
    for(auto obj : *MapManager::get()->getCurrentMap()->gameObjects){
        if(obj->behavior == address){
            return obj;
        }
    }

    return nullptr;
}
