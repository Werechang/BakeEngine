#include "GuiElement.h"

std::vector<GuiElement*> GuiElement::guiElements;
Matrix4 GLRenderer::guiProj;

/**
 *
 * @param xPos x Position in pixel coordinates (0-1600), from left
 * @param yPos y Position in pixel coordinates (0-900), from top
 * @param xSize x size going from left to right
 * @param ySize y size going from top to bottom
 * @param resizeWithScreen resize the element itself on screen resize. WARNING! This would also change the aspect ratio of the element itself, meaning that it can squish/stretch
 * @param bindTo if the element should stick do a specific edge on resize. 0 if none, look into the definitions in GuiElement.h
 */
GuiElement::GuiElement(float xPos, float yPos, float xSize, float ySize, bool resizeWithScreen, int bindTo) : xPos(xPos), yPos(yPos), xSize(xSize), ySize(ySize), resizeWithScreen(resizeWithScreen), bindTo(bindTo) {
    // TODO Change the way coordinates work
    /*
     * screen in normalized device coordinates
     * (-1,+1)|-----------------|(+1,+1)
     *        |                 |
     *        |                 |
     * (-1,-1)|-----------------|(+1,-1)
     */
    float vertices[] = {
            xPos,       yPos,          0.0f, 1.0f, // Top left
            xPos,       yPos+ySize,    0.0f, 0.0f, // Bottom left
            xPos+xSize, yPos+ySize,    1.0f, 0.0f, // Bottom right

            xPos,       yPos,          0.0f, 1.0f, // Top left
            xPos+xSize, yPos+ySize,    1.0f, 0.0f, // Bottom right
            xPos+xSize, yPos,          1.0f, 1.0f  // Top right
    };
    vao.bind();
    VertexBuffer vertexBuffer(vertices, sizeof(vertices));
    vertexBuffer.bind();
    VertexAttributes attribs;
    attribs.addAttribute<float>(2);
    attribs.addAttribute<float>(2);
    vao.addVertexBuffer(vertexBuffer, attribs);
    vao.unbind();
    vertexBuffer.unbind();
    guiElements.emplace_back(this);
    onResize(GLRenderer::renderer->width, GLRenderer::renderer->height);
}

void GuiElement::renderElement(GLShader &shader) {
    shader.uniformMatrix4fv("modelProj", modelProj);
    texture.bind(0);
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GuiElement::onResize(int width, int height) {
    // TODO Operations with model
    auto oldWidth = (float)GLRenderer::renderer->width;
    auto oldHeight = (float)GLRenderer::renderer->height;
    if (resizeWithScreen)
        model.scale((float)width/oldWidth, (float)height/oldHeight, 0);
    // scaling does change the position currently (meaning that it would not align with an edge/corner)
    modelProj = GLRenderer::guiProj * model;
}

void GuiElement::addChild(GuiElement* child) {
    children.emplace_back(child);
    child->parent = this;
}

bool GuiElement::hasParent() const {
    return parent != nullptr;
}

GuiElement *GuiElement::getParent() const {
    return parent;
}