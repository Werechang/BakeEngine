// Wir schaffen das, und dort, wo uns etwas im Wege steht, muss es überwunden werden.
#pragma once

#include <unordered_map>
#include "../../Util/Math/Matrix4.h"
#include "../../Util/LogHelperBE.h"
#include "glad/gl.h"

struct GLShaderSource {
    std::string vertexSource;
    std::string fragmentSource;
    std::string geometrySource;
};
class GLShader {
private:
    uint32_t programPtr;
    std::unordered_map<std::string, int> uniformLocationCache;

    GLShaderSource parseShader(const std::string& filePath) const;
    uint32_t compileShader(uint32_t shaderType, const char* source) const;
    uint32_t createProgram(const char *vertSource, const char *fragSource, const char* geometrySource = nullptr) const;

    int getUniformLocation(const std::string& name);
public:
    explicit GLShader(const std::string& filePath);
    ~GLShader();

    void uniform4f(const std::string& name, float a, float b, float c, float d);
    void uniform3f(const std::string& name, float a, float b, float c);
    void uniform3f(const std::string& name, const Vector3& vec);
    void uniform1f(const std::string& name, float a);
    void uniform1i(const std::string& name, int a);
    void uniformMatrix4fv(const std::string& name, const Matrix4& matrix);

    void bind() const;
    void unbind() const;
};
