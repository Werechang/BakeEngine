// Wir schaffen das, und dort, wo uns etwas im Wege steht, muss es überwunden werden.
#ifndef BAKEENGINE_GLSHADER_H
#define BAKEENGINE_GLSHADER_H

#include "glad/glad.h"
#include <string>

struct GLShaderSource {
    std::string vertexSource;
    std::string fragmentSource;
};
class GLShader {
private:
    unsigned int programPtr;

    GLShaderSource parseShader(const std::string& filePath);
    unsigned int compileShader(unsigned int shaderType, const char* source);
    unsigned int createProgram(const char *vertSource, const char *fragSource);
public:
    GLShader(const std::string& filePath);
    ~GLShader();
    void bind();
    void unbind();
    unsigned int getProgram();
};


#endif
