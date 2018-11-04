//
// Created by albert on 1/3/18.
//

#ifndef SIMPLE2DENGINE_MAP_H
#define SIMPLE2DENGINE_MAP_H

#include <string>
#include <regex>
#include <iostream>
#include "types.h"
#include "GameObject.h"
#include <chrono>
#include <functional>

namespace Simple2D{
    class Map {
    private:
        long getTimeInMs();
        std::string name;
        std::string path;
        long lastTimestamp;
        long lastFixedUpdate;
        std::unordered_map<int, std::function<Behavior*(void)>> currentlyLoadedCollection;
        void loadAttribute(Behavior* target, const std::string& type, const std::string& valueString, std::string symbol);

    public:
        Map();
        ~Map();

        int load(std::unordered_map<int, std::function<Behavior*(void)>> & collection, std::string path);
        GameObject * instanciatePrefab(std::string prefabCollectionPath, std::string prefabName, std::string newName);

        void remove();
        void updateAll();
        void setupAll();
        void fixedUpdateAll();
        void eventHandelAll(SDL_Event e);
        void renderAll(GLuint shaderProgramme);
        std::string getName();
        std::string getPath();

        std::vector<GameObject*>* gameObjects;
    };
}



#endif //SIMPLE2DENGINE_MAP_H
