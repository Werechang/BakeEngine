// Ich denke an dichte Fenster! Kein anderes Land kann so dichte und schöne Fenster bauen.
#pragma once

#include <string>
#include "../../Util/Math/Matrix4.h"
#include "GLShader.h"

class GLRenderer {
public:
    GLRenderer(const std::string& guiShaderPath, int width, int height);
    void draw();
    void resize(int newWidth, int newHeight);
private:
    int width;
    int height;

    GLShader guiShader;
    Matrix4 guiProj;
};
