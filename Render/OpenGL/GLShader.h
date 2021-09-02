// Wir schaffen das, und dort, wo uns etwas im Wege steht, muss es überwunden werden.
#ifndef BAKEENGINE_GLSHADER_H
#define BAKEENGINE_GLSHADER_H

#include "glad/glad.h"
#include <unordered_map>
#include "../../Util/Math/Matrix4.h"
#include "../../Util/LogHelperBE.h"

struct GLShaderSource {
    std::string vertexSource;
    std::string fragmentSource;
    std::string geometrySource;
};
class GLShader {
private:
    unsigned int programPtr;
    std::unordered_map<std::string, int> uniformLocationCache;

    GLShaderSource parseShader(const std::string& filePath);
    unsigned int compileShader(unsigned int shaderType, const char* source);
    unsigned int createProgram(const char *vertSource, const char *fragSource, const char* geometrySource = nullptr);

    int getUniformLocation(const std::string& name);
public:
    explicit GLShader(const std::string& filePath);
    ~GLShader();

    void uniform4f(const std::string& name, float a, float b, float c, float d);
    void uniform3f(const std::string& name, float a, float b, float c);
    void uniformVec3(const std::string& name, Vector3& vec);
    void uniform1f(const std::string& name, float a);
    void uniform1i(const std::string& name, int a);
    void uniformMatrix4fv(const std::string& name, Matrix4& matrix);

    void bind() const;
    void unbind() const;
};


#endif
