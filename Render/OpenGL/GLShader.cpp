// Wir schaffen das, und dort, wo uns etwas im Wege steht, muss es überwunden werden.
#include <sstream>
#include <fstream>
#include <iostream>
#include "GLShader.h"

GLShader::GLShader(const std::string &filePath) {
    GLShaderSource src = parseShader(filePath);
    programPtr = createProgram(src.vertexSource.c_str(), src.fragmentSource.c_str());
}

GLShader::~GLShader() {
    glDeleteProgram(programPtr);
}

GLShaderSource GLShader::parseShader(const std::string &filePath) {
    std::ifstream stream(filePath);

    std::string l;
    std::stringstream shaderSource[2];
    int shaderType = -1;
    while (std::getline(stream, l)) {
        if (l.find("#vertex") != std::string::npos) {
            shaderType = 0;
        } else if (l.find("#fragment") != std::string::npos) {
            shaderType = 1;
        } else {
            shaderSource[shaderType] << l << "\n";
        }
    }
    return {shaderSource[0].str(), shaderSource[1].str()};
}

unsigned int GLShader::compileShader(const unsigned int shaderType, const char* source) {
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Log errors
    int compileResult;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE) {
        char message[512];
        glGetShaderInfoLog(shader, 512, nullptr, message);
        std::string type;
        switch (shaderType) {
            case GL_FRAGMENT_SHADER: type = "fragment";break;
            case GL_VERTEX_SHADER: type = "vertex";break;
        }
        std::cerr << "[BakeEngine]:[Error] Could not compile " << type << " shader:" << std::endl;
        std::cerr << message << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

unsigned int GLShader::createProgram(const char* vertSource, const char* fragSource) {
    unsigned int shaderProgram = glCreateProgram();
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragSource);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    return shaderProgram;
}

int GLShader::getUniformLocation(const std::string &name) {
    if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
        return uniformLocationCache[name];
    }
    int location = glGetUniformLocation(programPtr, name.c_str());
    if (location == -1) {

        std::cerr << "[BakeEngine]:[Warning] Uniform " << name << " does not exist!" << std::endl;
    }
    uniformLocationCache[name] = location;
    return location;
}

void GLShader::bind() const {
    glUseProgram(programPtr);
}

void GLShader::unbind() {
    glUseProgram(0);
}

void GLShader::uniform4f(const std::string &name, float a, float b, float c, float d) {
    glUniform4f(getUniformLocation(name), a, b, c, d);
}

void GLShader::uniform3f(const std::string &name, float a, float b, float c) {
    glUniform3f(getUniformLocation(name), a, b, c);
}

void GLShader::uniform1f(const std::string &name, float a) {
    glUniform1f(getUniformLocation(name), a);
}

void GLShader::uniform1i(const std::string &name, int a) {
    glUniform1i(getUniformLocation(name), a);
}

void GLShader::uniformMatrix4fv(const std::string &name, Matrix4 &matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.ptr(0, 0));
}
