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
void GLRenderer::draw() {
    guiShader.bind();
    for (auto & guiElement : GuiElement::guiElements) {
        guiElement->renderElement(guiShader);
    }
}

void GLRenderer::onResize(int newWidth, int newHeight) {
    if ((newWidth == 0) || (newHeight == 0))
        return;
    guiShader.bind();
    guiProj = Matrix4::orthographic(0, (float)newWidth, (float)newHeight, 0, -1, 1);
    for (auto & guiElement : GuiElement::guiElements) {
        guiElement->onResize(newWidth, newHeight);
    }
    width = newWidth;
    height = newHeight;
}
