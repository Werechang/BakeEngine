// Ich denke an dichte Fenster! Kein anderes Land kann so dichte und schöne Fenster bauen.
#include "GLRenderer.h"

/* Right handed coordinate system
 *           +y -z
 *            | /
 *            |/
 * -x --------+--------- +x
 *           /|
 *          / |
 *        +z  -y
 */

GLRenderer::GLRenderer(const std::string& guiShaderPath, int width, int height) : guiShader(guiShaderPath), width(width), height(height) {
    guiProj = Matrix4::orthographic(0, (float)width, (float)height, 0, -1.0f, 1.0f);
    onResize(width, height);
}

void GLRenderer::draw() {
    guiShader.bind();
    for (auto & guiElement : guiElements) {
        if (guiElement->getVisible()) {
            guiElement->renderElement(guiShader);
        }
    }
}

void GLRenderer::onResize(int newWidth, int newHeight) {
    if ((newWidth == 0) || (newHeight == 0))
        return;
    guiShader.bind();
    guiProj = Matrix4::orthographic(0, (float)newWidth, (float)newHeight, 0, -1, 1);
    for (auto & guiElement : guiElements) {
        guiElement->onResize((float)newWidth, (float)newHeight, (float)width, (float)height, guiProj);
    }
    width = newWidth;
    height = newHeight;
}

const Matrix4 &GLRenderer::getMatrix() const {
    return guiProj;
}

int GLRenderer::getWidth() const {
    return width;
}

int GLRenderer::getHeight() const {
    return height;
}
