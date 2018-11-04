//h
// Created by albert on 1/4/18.
//

#ifndef SIMPLE2DENGINE_SIMPLE2D_H
#define SIMPLE2DENGINE_SIMPLE2D_H

#include <vector>
#include <string>
#include "GameObject.h"
#include "Behavior.h"

namespace Simple2D{
    Simple2D::GameObject* findGameObject(std::string name);
    Simple2D::GameObject* findGameObject(Behavior* address);
}


#endif //SIMPLE2DENGINE_SIMPLE2D_H
