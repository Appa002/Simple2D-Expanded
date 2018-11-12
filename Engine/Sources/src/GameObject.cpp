#include <iostream>
#include "../header/GameObject.h"
#include "../header/types.h"
#include "../header/MapManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Simple2D::GameObject::setupOpenglBuffer()
{
    glDeleteBuffers(1, posVbo);
    glDeleteBuffers(1, vtVbo);
    glDeleteBuffers(1, vao);

    glGenBuffers(1, posVbo);
    glGenBuffers(1, vtVbo);
    glGenVertexArrays(1, vao);

    GLfloat pos[]{
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, *posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

    GLfloat texcoords[]{
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, *vtVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);


    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *posVbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vtVbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);


}

Simple2D::GameObject::GameObject() {
    spriteHeight = new int(0);
    spriteWidth = new int(0);
    vao = new GLuint(0);
    posVbo = new GLuint(0);
    vtVbo = new GLuint(0);
}

Simple2D::GameObject::~GameObject() {
    if(behavior->amountOfAttributes() > 0){
        printf("[WARNING] Not all attributes of GameObject \"%s\"'s behavior have been removed. This causes a memory leak.\n", this->name.c_str());
    }

    glDeleteBuffers(1, posVbo);
    glDeleteBuffers(1, vtVbo);
    glDeleteBuffers(1, vao);

    delete spriteHeight;
    delete spriteWidth;
    delete vao;
    delete vtVbo;
    delete posVbo;
    delete behavior;
}

void Simple2D::GameObject::render(GLuint shaderProgramme) {
    glUseProgram(shaderProgramme);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.4);
    glEnable(GL_BLEND);
    glColor4f(1.0,1.0,1.0,1.0);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    if(!imageData)
        return;

    if(behavior->attributeExist("position")){
        GLint loc = glGetUniformLocation(shaderProgramme, "pos");
        if(loc != -1){
            float data[3];
            *data = behavior->getAttribute<Vec3>("position").x;
            *(data + 1) = behavior->getAttribute<Vec3>("position").y;
            *(data + 2) = behavior->getAttribute<Vec3>("position").z;
            glUniform3fv(loc, 1, data);
        }
    }
    else{
        GLint loc = glGetUniformLocation(shaderProgramme, "pos");
        if(loc != -1) {
            float data[3];
            *data = 0.0f;
            *(data + 1) = 0.0f;
            *(data + 2) = 0.0f;
            glUniform3fv(loc, 1, data);
        }
    }

    if(behavior->attributeExist("scale")){
        GLint loc = glGetUniformLocation(shaderProgramme, "scale");
        if(loc != -1){
            float data[3];
            *data = behavior->getAttribute<Vec3>("scale").x;
            *(data + 1) = behavior->getAttribute<Vec3>("scale").y;
            *(data + 2) = behavior->getAttribute<Vec3>("scale").z;
            glUniform3fv(loc, 1, data);
        }
    }
    else{
        GLint loc = glGetUniformLocation(shaderProgramme, "scale");
        if(loc != -1){
            float data[3];
            *data = 1;
            *(data + 1) = 1;
            *(data + 2) = 1;
            glUniform3fv(loc, 1, data);
        }
    }

    GameObject* camObj = findOtherGameObject("Camera");
    if(camObj != nullptr && camObj->behavior->attributeExist("position"))
    {
        GLint loc = glGetUniformLocation(shaderProgramme, "camPos");
        if(loc != -1){
            float data[3];
            *data = camObj->behavior->getAttribute<Vec3>("position").x;
            *(data + 1) = camObj->behavior->getAttribute<Vec3>("position").y;
            *(data + 2) = camObj->behavior->getAttribute<Vec3>("position").z;
            glUniform3fv(loc, 1, data);
        }
    }
    else{
        GLint loc = glGetUniformLocation(shaderProgramme, "camPos");
        if(loc != -1){
            float data[3] {0.0f, 0.0f, 0.0f};
            glUniform3fv(loc, 1, data);
        }
    }

    if (camObj != nullptr && camObj->behavior->attributeExist("scale"))
    {
        GLint loc = glGetUniformLocation(shaderProgramme, "camScale");
        if (loc != -1) {
            float data[3];
            *data = camObj->behavior->getAttribute<Vec3>("scale").x;
            *(data + 1) = camObj->behavior->getAttribute<Vec3>("scale").y;
            *(data + 2) = camObj->behavior->getAttribute<Vec3>("scale").z;
            glUniform3fv(loc, 1, data);
        }
    }
    else {
        GLint loc = glGetUniformLocation(shaderProgramme, "camScale");
        if (loc != -1) {
            float data[3]{ 1.0f, 1.0f, 1.0f };
            glUniform3fv(loc, 1, data);
        }
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(*vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

Simple2D::GameObject *Simple2D::GameObject::findOtherGameObject(std::string name) {
    for(auto gObj : *MapManager::get()->getCurrentMap()->gameObjects){
        if(gObj->name == name){
            return gObj;
        }
    }

    return nullptr;
}

void Simple2D::GameObject::loadNewSprite(std::string path) {
    setupOpenglBuffer();

    int n;
    imageData = stbi_load(path.c_str(), spriteWidth, spriteHeight, &n, 4);

    if(!imageData){
        return;
    }

    // Check if dimensions are not a power of two.
    // Older GPUs can't handel textures which are not a power of two.
    if((*spriteWidth & (*spriteWidth - 1)) != 0 || (*spriteWidth & (*spriteHeight - 1)) != 0){
        printf("[WARNING] Dimensions not a power of two for GameObject \"%s\" \n", name.c_str());
    }

    // Copy Sprite into opengl texture
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *spriteWidth, *spriteHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    delete imageData;
}

void Simple2D::GameObject::remove() {
    auto vec = MapManager::get()->getCurrentMap()->gameObjects;

    for(auto objAddress : *vec){
        if(objAddress == this){
            this->behavior->onRemoval();
            this->markedForDeletion = true;
            return;
        }
    }
}

bool Simple2D::GameObject::isMarkedForDeletion() {
    return this->markedForDeletion;
}
