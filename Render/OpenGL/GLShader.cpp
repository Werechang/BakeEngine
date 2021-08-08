// Wir schaffen das, und dort, wo uns etwas im Wege steht, muss es überwunden werden.
#include <sstream>
#include <fstream>
#include <iostream>
#include "GLShader.h"

GLShader::GLShader(const std::string& filePath) {
    GLShaderSource src = parseShader(filePath);
    programPtr = createProgram(src.vertexSource.c_str(), src.fragmentSource.c_str());
}

GLShader::~GLShader() {
    glDeleteProgram(programPtr);
}

GLShaderSource GLShader::parseShader(const std::string& filePath) {
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
    if (!compileResult) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);
        std::string type;
        switch (shaderType) {
            case 0x8B30: type = "fragment";break;
            case 0x8B31: type = "vertex";break;
        }
        std::cerr << "Could not compile " << type << " shader:" << std::endl;
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

int GLShader::getUniformLocation(const std::string& name) {
    if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
        return uniformLocationCache[name];
    }
    int location = glGetUniformLocation(programPtr, name.c_str());
    if (location == -1) {
        std::cerr << "[Warning] Uniform location does not exist!" << std::endl;
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

int GLShader::getAttribLocation() const {
    return glGetAttribLocation(programPtr, "position");
}

void GLShader::uniform4f(const std::string &name, float a, float b, float c, float d) {
    glUniform4f(getUniformLocation(name), a, b, c, d);
}
