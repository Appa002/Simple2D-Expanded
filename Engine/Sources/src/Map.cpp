//
// Created by albert on 1/3/18.
//

#include "../header/Map.h"
#include "../header/Behavior.h"
#include "../header/GameObjectRenderer.h"

#include <fstream>
#include <rapidxml.hpp>
#include <sstream>

using namespace Simple2D;

Simple2D::Map::Map(){
    gameObjects = new std::vector<GameObject*>;
    this->lastTimestamp = getTimeInMs();
}

Simple2D::Map::~Map() {
    if(gameObjects){
        remove();
    }
}

int Simple2D::Map::load(std::unordered_map<int, std::function<Behavior*(void)>> & collection, std::string path) {
    this->currentlyLoadedCollection = collection;
    this->path = path;
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    char* text = new char[buffer.str().size() + 1]; // TODO: This is terribly in efficent.
    for (int i = 0; i < buffer.str().size(); i++) {
        text[i] = buffer.str().at(i);
    }
    text[buffer.str().size()] = '\0';

    using namespace rapidxml;
    xml_document<char> doc;    // character type defaults to char
    doc.parse<0>(text);    // 0 means default parse flags


    this->name = doc.first_node()->first_node("name")->value();

    for (auto objNode = doc.first_node()->first_node("gameobjects")->first_node(); objNode; objNode = objNode->next_sibling() ) {
        // Looping through all `obj` nodes in the xml.....
        auto gObj = new GameObject();
        gObj->name = objNode->first_node("name")->value();
        int behaviorIdx = std::stoi(objNode->first_node("behavior")->value());
        gObj->behavior = collection.at(behaviorIdx) ();
        gObj->behaviorIdx = behaviorIdx;


        for (auto attrNode = objNode->first_node("attributes")->first_node(); attrNode; attrNode = attrNode->next_sibling()) {
            // Looping through all `attr` nodes in the current `obj` node in the xml.....
            std::string type = attrNode->first_node("type")->value();
            std::string valueString = attrNode->first_node("value")->value();
            std::string symbol = attrNode->first_node("symbol")->value();

            loadAttribute(gObj->behavior, type, valueString, symbol);

        }

        this->gameObjects->push_back(gObj);
        gObj->behavior->init();

    }

    delete text;
    return 0;
}

GameObject * Simple2D::Map::instanciatePrefab(std::string prefabCollectionPath, std::string prefabName, std::string newName)
{
    std::ifstream t(prefabCollectionPath);
    std::stringstream buffer;
    buffer << t.rdbuf();
    char* text = new char[buffer.str().size() + 1]; // TODO: This is terribly in efficent.
    for (int i = 0; i < buffer.str().size(); i++) {
        text[i] = buffer.str().at(i);
    }
    text[buffer.str().size()] = '\0';

    using namespace rapidxml;
    xml_document<char> doc;    // character type defaults to char
    doc.parse<0>(text);    // 0 means default parse flags

    bool found = false;
    auto objNode = doc.first_node()->first_node("gameobjects")->first_node();
    for (; objNode; objNode = objNode->next_sibling()) {
        // Looping through all `obj` nodes in the xml.....
        if (objNode->first_node("name")->value() == prefabName) {
            found = true;
            break;
        }
    }

    if (!found)
        throw std::runtime_error("Trying to instanciate none existend prefab: \"" + prefabName + "\".");

    auto gObj = new GameObject;
    gObj->name = objNode->first_node("name")->value();
    int behaviorIdx = std::stoi(objNode->first_node("behavior")->value());
    gObj->behavior = currentlyLoadedCollection.at(behaviorIdx) ();
    gObj->behaviorIdx = behaviorIdx;


    for (auto attrNode = objNode->first_node("attributes")->first_node(); attrNode; attrNode = attrNode->next_sibling()) {
        // Looping through all `attr` nodes in the `obj` node in the xml.....
        std::string type = attrNode->first_node("type")->value();
        std::string valueString = attrNode->first_node("value")->value();
        std::string symbol = attrNode->first_node("symbol")->value();

        loadAttribute(gObj->behavior, type, valueString, symbol);
    }

    this->gameObjects->push_back(gObj);
    gObj->behavior->init();
    gObj->behavior->setup();
    return gObj;
}

void Simple2D::Map::loadAttribute(Behavior * target, const std::string & type, const std::string & valueString, std::string symbol)
{
    using namespace Simple2D;
    //TODO: Make this a enum lookup table...... for performance
    if (type == "string") {
        std::string* v = new std::string(valueString);
        target->addAttribute<std::string>(symbol, v);
    }
    else if (type == "int") {
        int* v = new int(std::stoi(valueString));
        target->addAttribute<int>(symbol, v);
    }
    else if (type == "long") {
        long* v = new long(std::stol(valueString));
        target->addAttribute<long>(symbol, v);
    }
    else if (type == "unsigned int") {
        unsigned int* v = new unsigned int(std::stoi(valueString));
        target->addAttribute<unsigned int>(symbol, v);
    }
    else if (type == "unsigned long") {
        unsigned long* v = new unsigned long(std::stol(valueString));
        target->addAttribute<unsigned long>(symbol, v);
    }
    else if (type == "float") {
        float* v = new float(std::stof(valueString));
        target->addAttribute<float>(symbol, v);
    }
    else if (type == "double") {
        double* v = new double(std::stod(valueString));
        target->addAttribute<double>(symbol, v);
    }
    else if (type == "vec3") {
        vec3* v = new vec3(sto3(valueString));
        target->addAttribute<vec3>(symbol, v);
    }
    else if (type == "vec2") {
        vec2* v = new vec2(sto2(valueString));
        target->addAttribute<vec2>(symbol, v);
    }
    else if (type == "dvec3") {
        dvec3* v = new dvec3(stod3(valueString));
        target->addAttribute<dvec3>(symbol, v);
    }
    else if (type == "dvec2") {
        dvec2* v = new dvec2(stod2(valueString));
        target->addAttribute<dvec2>(symbol, v);
    }
}

void Simple2D::Map::remove() {
    for(auto* g : *gameObjects){
        g->behavior->onRemoval();
    }
    this->gameObjects->clear();
    delete gameObjects;
    gameObjects = nullptr;
}

void Simple2D::Map::updateAll() {
    long curTime = getTimeInMs();
    long deltaTime = curTime - lastTimestamp;
    lastTimestamp = getTimeInMs();
   // std::cout << deltaTime << std::endl; // Before Optimization 16/17 ms.

    size_t length = this->gameObjects->size();
    for(size_t i = 0; i < length; i++){
        try {
            try {
                this->gameObjects->at(i)->behavior->update(deltaTime);
            } catch (std::exception& e){
                printf("[ERROR] GameObject \"%s\" threw error while executing \"update()\", error: \n%s \n", name.c_str(), e.what());
            }
        } catch (...){
            printf("[ERROR] GameObject \"%s\" threw error while executing \"update()\",this error is not of type std::exception no further information can be provided  \n", name.c_str());
        }
    }

    for(size_t idx = 0; idx < this->gameObjects->size(); idx++){
        auto g = this->gameObjects->at(idx);
        if(g->isMarkedForDeletion()){
            this->gameObjects->erase(this->gameObjects->begin() + idx);
            delete g;
        }
    }

}

void Simple2D::Map::renderAll() {
    for(auto* gameObject : *this->gameObjects){
     //   gameObject->render(shaderProgramme);
        GameObjectRenderer::get()->render(*gameObject);
    }
}

void Simple2D::Map::setupAll() {
    size_t length = this->gameObjects->size();
    for(size_t i = 0; i < length; i++){
        try {
            try {
                this->gameObjects->at(i)->behavior->setup();
            } catch (std::exception& e){
                printf("[ERROR] GameObject \"%s\" threw error while executing \"setup()\", error: \n%s \n", name.c_str(), e.what());
            }
        } catch (...){
            printf("[ERROR] GameObject \"%s\" threw error while executing \"setup()\",this error is not of type std::exception no further information can be provided  \n", name.c_str());
        }
    }

    for(size_t idx = 0; idx < this->gameObjects->size(); idx++){
        auto g = this->gameObjects->at(idx);
        if(g->isMarkedForDeletion()){
            this->gameObjects->erase(this->gameObjects->begin() + idx);
            delete g;
        }
    }
}

void Simple2D::Map::eventHandelAll(SDL_Event e) {
    size_t length = this->gameObjects->size();
    for(size_t i = 0; i < length; i++){
        try {
            try {
                this->gameObjects->at(i)->behavior->onEvent(e);
            } catch (std::exception& e){
                printf("[ERROR] GameObject \"%s\" threw error while executing \"onEvent()\", error: \n%s \n", name.c_str(), e.what());
            }
        } catch (...){
            printf("[ERROR] GameObject \"%s\" threw error while executing \"onEvent()\",this error is not of type std::exception no further information can be provided  \n", name.c_str());
        }
    }

    for(size_t idx = 0; idx < this->gameObjects->size(); idx++){
        auto g = this->gameObjects->at(idx);
        if(g->isMarkedForDeletion()){
            this->gameObjects->erase(this->gameObjects->begin() + idx);
            delete g;
        }
    }
}

std::string Simple2D::Map::getName() {
    return this->name;
}

std::string Simple2D::Map::getPath()
{
    return this->path;
}

void Simple2D::Map::fixedUpdateAll() {
    long curTime = getTimeInMs();
    if(curTime - lastFixedUpdate < 20)
        return;

 //   if(curTime - lastFixedUpdate > 20 + 5)
  //      std::cout << "[WARNING] fixedUpdate() called with " << getTimeInMs() - lastFixedUpdate << " milliseconds of delay, normally called with 20 milliseconds of delay." << std::endl;

    lastFixedUpdate = getTimeInMs();

    size_t length = this->gameObjects->size();
    for(size_t i = 0; i < length; i++){
        try {
            try {
                this->gameObjects->at(i)->behavior->fixedUpdate();
            } catch (std::exception& e){
                printf("[ERROR] GameObject \"%s\" threw error while executing \"fixedUpdate()\", error: \n%s \n", name.c_str(), e.what());
            }
        } catch (...){
            printf("[ERROR] GameObject \"%s\" threw error while executing \"fixedUpdate()\", this error is not of type std::exception no further information can be provided.s\n", name.c_str());
        }
    }

    for(size_t idx = 0; idx < this->gameObjects->size(); idx++){
        auto g = this->gameObjects->at(idx);
        if(g->isMarkedForDeletion()){
            this->gameObjects->erase(this->gameObjects->begin() + idx);
            delete g;
        }
    }

}

long Simple2D::Map::getTimeInMs() {
    return std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();
}