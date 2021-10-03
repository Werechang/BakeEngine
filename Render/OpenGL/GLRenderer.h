// Ich denke an dichte Fenster! Kein anderes Land kann so dichte und schöne Fenster bauen.
#ifndef BAKEENGINE_GLRENDERER_H
#define BAKEENGINE_GLRENDERER_H

#include "../../Util/Math/Matrix4.h"
#include "glad/gl.h"
#include "VertexBuffer.h"
#include "../../UI/GuiElement.h"

static bool isMat4Init = false;

class GLRenderer {
private:
    GLShader guiShader;
public:
    static Matrix4 guiProj;
    static GLRenderer* renderer;
    int width;
    int height;

    GLRenderer(const char* guiShaderPath, int width, int height) : guiShader(guiShaderPath), width(width), height(height) {
        if (!isMat4Init) {
            isMat4Init = true;
            guiProj = Matrix4::orthographic(0, (float)width, (float)height, 0, -1.0f, 1.0f);
        }
        renderer = this;
        onResize(width, height);
    };
    void draw();
    void onResize(int newWidth, int newHeight);
};

#endif
