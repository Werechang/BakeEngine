// Ich denke an dichte Fenster! Kein anderes Land kann so dichte und schöne Fenster bauen.
#pragma once

#include "../../UI/GuiElement.h"

class GLRenderer {
public:
    GLRenderer(const std::string& guiShaderPath, int width, int height);
    void draw();
    void onResize(int newWidth, int newHeight);

    const Matrix4& getMatrix() const;
    int getWidth() const;
    int getHeight() const;
private:
    int width;
    int height;

    GLShader guiShader;
    Matrix4 guiProj;

    std::vector<GuiElement*> guiElements;
};
