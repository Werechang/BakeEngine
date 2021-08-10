// Wir schaffen das, und dort, wo uns etwas im Wege steht, muss es überwunden werden.
#ifndef BAKEENGINE_GLSHADER_H
#define BAKEENGINE_GLSHADER_H

#include "glad/glad.h"
#include <string>
#include <unordered_map>
#include "../../Util/Math/Matrix4.h"

struct GLShaderSource {
    std::string vertexSource;
    std::string fragmentSource;
};
class GLShader {
private:
    unsigned int programPtr;
    std::unordered_map<std::string, int> uniformLocationCache;

    static GLShaderSource parseShader(const std::string& filePath);
    static unsigned int compileShader(unsigned int shaderType, const char* source);
    static unsigned int createProgram(const char *vertSource, const char *fragSource);

    int getUniformLocation(const std::string& name);
public:
    explicit GLShader(const std::string& filePath);
    ~GLShader();

    void uniform4f(const std::string& name, float a, float b, float c, float d);
    void uniform3f(const std::string& name, float a, float b, float c);
    void uniform1i(const std::string& name, int a);
    void uniformMatrix4fv(const std::string& name, Matrix4& matrix);

    void bind() const;
    static void unbind();
    int getAttribLocation(const char* name) const;
};


#endif
