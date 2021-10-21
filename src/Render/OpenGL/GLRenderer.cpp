// Ich denke an dichte Fenster! Kein anderes Land kann so dichte und schöne Fenster bauen.
#include "GLRenderer.h"
#include "../../Application/Application.h"

/* Right handed coordinate system
 *           +y -z
 *            | /
 *            |/
 * -x --------+--------- +x
 *           /|
 *          / |
 *        +z  -y
 */

GLRenderer::GLRenderer(const std::string& guiShaderPath, int width, int height) : width(width), height(height),
                                                                                  guiShader(guiShaderPath) {
    guiProj = Matrix4::orthographic(0, (float)width, (float)height, 0, -1.0f, 1.0f);
    guiShader.bind();
    guiShader.uniform1i("tex", 0);
    guiShader.unbind();
    resize(width, height);
}

void GLRenderer::draw() {
    guiShader.bind();
    for (auto & guiElement : Application::guiManagerPtr->elements) {
        if (guiElement->getVisible()) {
            guiElement->renderElement(guiShader);
        }
    }
    guiShader.unbind();
}

void GLRenderer::resize(int newWidth, int newHeight) {
    if ((newWidth == 0) || (newHeight == 0))
        return;
    guiProj = Matrix4::orthographic(0, (float)newWidth, (float)newHeight, 0, -1, 1);
    for (auto & guiElement : Application::guiManagerPtr->elements) {
        guiElement->resize((float) newWidth, (float) newHeight, (float) width, (float) height, guiProj);
    }
    width = newWidth;
    height = newHeight;
}
