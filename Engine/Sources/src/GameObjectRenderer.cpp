#include "..\header\GameObjectRenderer.h"
#include "..\header\Simple2D.h"

#include <fstream>

Simple2D::GameObjectRenderer::~GameObjectRenderer()
{
    glDeleteBuffers(1, &posVbo);
    glDeleteBuffers(1, &vtVbo);
    glDeleteBuffers(1, &vao);
}

void Simple2D::GameObjectRenderer::setupOpenglBuffer()
{
    glDeleteBuffers(1, &posVbo);
    glDeleteBuffers(1, &vtVbo);
    glDeleteBuffers(1, &vao);

    glGenBuffers(1, &posVbo);
    glGenBuffers(1, &vtVbo);
    glGenVertexArrays(1, &vao);

    GLfloat pos[]{
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

    GLfloat texcoords[]{
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vtVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);


    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vtVbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

}

void Simple2D::GameObjectRenderer::setupShaders()
{
    std::string vertex_shader;
    std::string fragment_shader;

    std::ifstream vsFile;
    std::ifstream fsFile;

    vsFile.open("./vertex.shader", std::ios::in);
    fsFile.open("./fragment.shader", std::ios::in);


    vsFile.seekg(0, std::ios::end);
    fsFile.seekg(0, std::ios::end);

    vertex_shader.reserve(vsFile.tellg());
    fragment_shader.reserve(fsFile.tellg());

    vsFile.seekg(0, std::ios::beg);
    fsFile.seekg(0, std::ios::beg);

    vertex_shader.assign((std::istreambuf_iterator<char>(vsFile)),
        std::istreambuf_iterator<char>());

    fragment_shader.assign((std::istreambuf_iterator<char>(fsFile)),
        std::istreambuf_iterator<char>());

    const char* tmp;

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    tmp = vertex_shader.c_str();
    glShaderSource(vs, 1, &tmp, NULL);
    glCompileShader(vs);

    GLint succses = 0;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &succses);
    if (!succses) {
        GLint logSize = 0;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &logSize);
        char* buffer = new char[logSize];
        glGetShaderInfoLog(vs, logSize, nullptr, buffer);
        std::cout << "Vertex Shader Compile Errors: " << "\n";
        std::cout << buffer << std::endl;
        delete buffer;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    tmp = fragment_shader.c_str();
    glShaderSource(fs, 1, &tmp, NULL);
    glCompileShader(fs);

    succses = 0;
    glGetShaderiv(fs, GL_COMPILE_STATUS, &succses);
    if (!succses) {
        GLint logSize = 0;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &logSize);
        char* buffer = new char[logSize];
        glGetShaderInfoLog(fs, logSize, nullptr, buffer);
        std::cout << "Fragment Shader Compile Errors: " << "\n";
        std::cout << buffer << std::endl;
        delete buffer;
    }

    shaderProgramme = glCreateProgram();
    glAttachShader(shaderProgramme, vs);
    glAttachShader(shaderProgramme, fs);
    glLinkProgram(shaderProgramme);


}

void Simple2D::GameObjectRenderer::render(const GameObject & gameObject)
{
    if (!gameObject.shallRender())
        return;

    glUseProgram(shaderProgramme);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.4);
    glEnable(GL_BLEND);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (gameObject.behavior->attributeExist("position")) {
        GLint loc = glGetUniformLocation(shaderProgramme, "pos");
        if (loc != -1) {
            float data[3];
            *data = gameObject.behavior->getAttribute<Vec3>("position").x;
            *(data + 1) = gameObject.behavior->getAttribute<Vec3>("position").y;
            *(data + 2) = gameObject.behavior->getAttribute<Vec3>("position").z;
            glUniform3fv(loc, 1, data);
        }
    }
    else {
        GLint loc = glGetUniformLocation(shaderProgramme, "pos");
        if (loc != -1) {
            float data[3];
            *data = 0.0f;
            *(data + 1) = 0.0f;
            *(data + 2) = 0.0f;
            glUniform3fv(loc, 1, data);
        }
    }

    if (gameObject.behavior->attributeExist("scale")) {
        GLint loc = glGetUniformLocation(shaderProgramme, "scale");
        if (loc != -1) {
            float data[3];
            *data = gameObject.behavior->getAttribute<Vec3>("scale").x;
            *(data + 1) = gameObject.behavior->getAttribute<Vec3>("scale").y;
            *(data + 2) = gameObject.behavior->getAttribute<Vec3>("scale").z;
            glUniform3fv(loc, 1, data);
        }
    }
    else {
        GLint loc = glGetUniformLocation(shaderProgramme, "scale");
        if (loc != -1) {
            float data[3];
            *data = 1;
            *(data + 1) = 1;
            *(data + 2) = 1;
            glUniform3fv(loc, 1, data);
        }
    }

    GameObject* camObj = findGameObject("Camera");
    if (camObj != nullptr && camObj->behavior->attributeExist("position"))
    {
        GLint loc = glGetUniformLocation(shaderProgramme, "camPos");
        if (loc != -1) {
            float data[3];
            *data = camObj->behavior->getAttribute<Vec3>("position").x;
            *(data + 1) = camObj->behavior->getAttribute<Vec3>("position").y;
            *(data + 2) = camObj->behavior->getAttribute<Vec3>("position").z;
            glUniform3fv(loc, 1, data);
        }
    }
    else {
        GLint loc = glGetUniformLocation(shaderProgramme, "camPos");
        if (loc != -1) {
            float data[3]{ 0.0f, 0.0f, 0.0f };
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
   
    glBindTexture(GL_TEXTURE_2D, gameObject.getTexture());
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);


}

void Simple2D::GameObjectRenderer::setShaderProgramme(GLuint shaderProgramme)
{
    this->shaderProgramme = shaderProgramme;
}
