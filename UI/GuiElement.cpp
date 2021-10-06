#include "GuiElement.h"

std::vector<GuiElement*> GuiElement::guiElements;
Matrix4 GLRenderer::guiProj;
// TODO Input
/**
 *
 * @param xPos x Position in pixel coordinates, from left
 * @param yPos y Position in pixel coordinates, from top
 * @param xSize x size going from left to right
 * @param ySize y size going from top to bottom
 * @param scalingAxis option for how the scaling of this element behaves. GUI_NO_AXIS scales with the aspect ratio,
 * meaning that the element can squish/stretch
 * GUI_AXIS_X and GUI_AXIS_Y scales by changing the width/height of the window
 * @param alignWith if the element should stick do a specific edge on resize. 0 if none, look into the definitions in GuiElement.h
 */
GuiElement::GuiElement(float xPos, float yPos, float xSize, float ySize, int scalingAxis, int alignWith) : xPos(xPos), yPos(yPos), xSize(xSize), ySize(ySize), scalingAxis(scalingAxis), alignWith(alignWith) {
    /*
     * screen in normalized device coordinates
     * (-1,+1)|-----------------|(+1,+1)
     *        |                 |
     *        |                 |
     * (-1,-1)|-----------------|(+1,-1)
     */
    float vertices[] = {
            0,       0,          0.0f, 1.0f, // Top left
            0,       ySize,    0.0f, 0.0f, // Bottom left
            xSize, ySize,    1.0f, 0.0f, // Bottom right

            0,       0,          0.0f, 1.0f, // Top left
            xSize, ySize,    1.0f, 0.0f, // Bottom right
            xSize, 0,          1.0f, 1.0f  // Top right
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
    model.translate(xPos, yPos, 0);
    onResize(GLRenderer::renderer->width, GLRenderer::renderer->height);
}

void GuiElement::renderElement(GLShader &shader) {
    shader.uniformMatrix4fv("modelProj", modelProj);
    texture.bind(0);
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GuiElement::onResize(int width, int height) {
    auto oldWidth = (float)GLRenderer::renderer->width;
    auto oldHeight = (float)GLRenderer::renderer->height;

    float xScale = 1, yScale = 1;
    if (scalingAxis > 0) {
        switch (scalingAxis) {
            case GUI_SCALE_NO_AXIS:
                xScale = (float)width/oldWidth;
                yScale = (float)height/oldHeight;
                break;
            case GUI_AXIS_X:
                xScale = (float)width/oldWidth;
                yScale = (float)width/oldWidth;
                break;
            case GUI_AXIS_Y:
                xScale = (float)height/oldHeight;
                yScale = (float)height/oldHeight;
                break;
        }
        model.scale(xScale, yScale, 1);
    }
    float newXSize = xScale*xSize;
    float newYSize = yScale*ySize;

    float newX = xPos, newY = yPos;
    switch (alignWith) {
        //                                      |--Distance-to-edge---|  \/ to get the top left
        // This is one of the algorithms: width-(oldWidth-(xPos+xSize))-xSize;
        case GUI_NONE: break;
        case GUI_TOP_CENTER:
            newX = (float)width/oldWidth*(xPos+xSize/2)-newXSize/2;
            break;
        case GUI_TOP_RIGHT:
            newX = (float)width-oldWidth+xPos+xSize-newXSize;
            break;
        case GUI_RIGHT_CENTER:
            newX = (float)width-oldWidth+xPos+xSize-newXSize;
            newY = (float)height/oldHeight*(yPos+ySize/2)-newYSize/2;
            break;
        case GUI_BOTTOM_RIGHT:
            newX = (float)width-oldWidth+xPos+xSize-newXSize;
            newY = (float)height-oldHeight+yPos+ySize-newYSize;
            break;
        case GUI_BOTTOM_CENTER:
            newX = (float)width/oldWidth*(xPos+xSize/2)-newXSize/2;
            newY = (float)height-oldHeight+yPos+ySize-newYSize;
            break;
        case GUI_BOTTOM_LEFT:
            newY = (float)height-oldHeight+yPos+ySize-newYSize;
            break;
        case GUI_LEFT_CENTER:
            newY = (float)height/oldHeight*(yPos+ySize/2)-newYSize/2;
            break;
        case GUI_TOP_LEFT: break;
        case GUI_CENTER:
            newX = (float)width/oldWidth*(xPos+xSize/2)-newXSize/2;
            newY = (float)height/oldHeight*(yPos+ySize/2)-newYSize/2;
            break;
    }

    model.translate(newX-xPos, newY-yPos, 0);
    xPos = newX;
    yPos = newY;
    xSize = newXSize;
    ySize = newYSize;
    modelProj = GLRenderer::guiProj * model;
}

void GuiElement::addChild(GuiElement* element) {
    // TODO children (with updatePos)
    if (element && !element->parent && (this->parent != element)) {
        children.emplace_back(element);
        element->parent = this;
    } else {
        LogHelperBE::pushName("GuiElement");
        LogHelperBE::error("Element does already have a parent or is nullptr!");
        LogHelperBE::popName();
    }
}

void GuiElement::setPos(float x, float y) {
    model.translate(x-xPos, y-yPos, 0);
    modelProj = GLRenderer::guiProj * model;
    xPos = x;
    yPos = y;
}

bool GuiElement::isMouseHover(int mouseX, int mouseY) const {
    int guiPosX = (int)std::round(xPos);
    int guiPosY = (int)std::round(yPos);
    int guiSizeX = (int)std::round(xSize);
    int guiSizeY = (int)std::round(ySize);

    if ((mouseX > guiPosX) && (mouseY > guiPosY) && (mouseX < (guiPosX+guiSizeX)) && (mouseY < (guiPosY+guiSizeY))) {
        return true;
    }
    return false;
}

bool GuiElement::hasParent() const {
    return parent != nullptr;
}

GuiElement *GuiElement::getParent() const {
    return parent;
}