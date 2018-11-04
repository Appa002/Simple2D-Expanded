//
// Created by albert on 1/9/18.
//

#include "../header/MapManager.h"

Simple2D::Map* Simple2D::MapManager::getCurrentMap() {
    return this->curMap;
}

int Simple2D::MapManager::loadMap(std::unordered_map<int, std::function<Behavior* (void)>> collection, std::string path) {
    this->curMap = new Map();
    this->curMap->load(collection, path);
    this->curMap->setupAll();
    return 0;
}

int Simple2D::MapManager::unloadMap() {
    this->curMap->remove();
    delete curMap;
    curMap = nullptr;

    return 0;
}