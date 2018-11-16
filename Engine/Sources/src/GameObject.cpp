#include <iostream>
#include "../header/GameObject.h"
#include "../header/types.h"
#include "../header/MapManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Simple2D::GameObject::GameObject()
{
}

Simple2D::GameObject::~GameObject() {
    if(behavior->amountOfAttributes() > 0){
        printf("[WARNING] Not all attributes of GameObject \"%s\"'s behavior have been removed. This causes a memory leak.\n", this->name.c_str());
    }

    delete behavior;
}

void Simple2D::GameObject::loadNewSprite(std::string path) {
    int n;
    int spriteWidth;
    int spriteHeight;
    
    unsigned const char* imageData = stbi_load(path.c_str(), &spriteWidth, &spriteHeight, &n, 4);

    if(!imageData){
        return;
    }
    this->hasTextureLoaded = true;

    // Check if dimensions are not a power of two.
    // Older GPUs can't handel textures which are not a power of two.
    if((spriteWidth & (spriteWidth - 1)) != 0 || (spriteWidth & (spriteHeight - 1)) != 0){
        printf("[WARNING] Dimensions not a power of two for GameObject \"%s\" \n", name.c_str());
    }

    // Copy Sprite into opengl texture
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spriteWidth, spriteHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    delete imageData;
}

void Simple2D::GameObject::remove() {

    this->behavior->onRemoval();
    this->markedForDeletion = true;
}

const GLuint& Simple2D::GameObject::getTexture() const
{
    return texture;
}

const bool & Simple2D::GameObject::shallRender() const
{
    return hasTextureLoaded;
}

bool Simple2D::GameObject::isMarkedForDeletion() {
    return this->markedForDeletion;
}
