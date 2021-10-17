// Wir schaffen das, und dort, wo uns etwas im Wege steht, muss es überwunden werden.
#include <sstream>
#include <fstream>
#include "GLShader.h"
#include "../../Application/Application.h"

GLShader::GLShader(const std::string &filePath) {
    LogHelperBE::pushName("Shader");
    GLShaderSource src = parseShader(Application::absolutePath + filePath);
    programPtr = createProgram(src.vertexSource.c_str(), src.fragmentSource.c_str(), src.geometrySource.empty() ? nullptr : src.geometrySource.c_str());
    LogHelperBE::popName();
}

GLShader::~GLShader() {
    glDeleteProgram(programPtr);
}

GLShaderSource GLShader::parseShader(const std::string &filePath) const{
    std::ifstream stream(filePath);

    std::string l;
    std::stringstream shaderSource[3];
    int shaderType = -1;
    while (std::getline(stream, l)) {
        if (l.find("#vertex") != std::string::npos) {
            shaderType = 0;
        } else if (l.find("#fragment") != std::string::npos) {
            shaderType = 1;
        } else if (l.find("#geometry") != std::string::npos) {
            shaderType = 2;
        } else {
            shaderSource[shaderType] << l << "\n";
        }
    }
    return {shaderSource[0].str(), shaderSource[1].str(), shaderSource[2].str()};
}

unsigned int GLShader::compileShader(const unsigned int shaderType, const char* source) const{
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
            case GL_GEOMETRY_SHADER: type = "geometry";break;
        }
        LogHelperBE::pushName("Shader");
        LogHelperBE::error("Could not compile " + std::string(type) + " shader\n" + message);
        LogHelperBE::popName();
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

unsigned int GLShader::createProgram(const char* vertSource, const char* fragSource, const char* geometrySource) const{
    unsigned int shaderProgram = glCreateProgram();
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragSource);
    if (geometrySource != nullptr) {
        unsigned int geometryShader = compileShader(GL_GEOMETRY_SHADER, geometrySource);
        glAttachShader(shaderProgram, geometryShader);
    }

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int GLShader::getUniformLocation(const std::string &name) {
    if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
        return uniformLocationCache[name];
    }
    int location = glGetUniformLocation(programPtr, name.c_str());

    if (location == -1) {
        LogHelperBE::pushName("Shader");
        LogHelperBE::error("Uniform " + std::string(name) + " does not exist!");
        LogHelperBE::popName();
    }
    uniformLocationCache[name] = location;
    return location;
}

void GLShader::bind() const {
    glUseProgram(programPtr);
}

void GLShader::unbind() const {
    glUseProgram(0);
}

void GLShader::uniform4f(const std::string &name, float a, float b, float c, float d) {
    glUniform4f(getUniformLocation(name), a, b, c, d);
}

void GLShader::uniform3f(const std::string &name, float a, float b, float c) {
    glUniform3f(getUniformLocation(name), a, b, c);
}

void GLShader::uniformVec3(const std::string &name, Vector3 &vec) {
    glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
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