#include "GuiElement.h"
#include "../Application/Application.h"

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
GuiElement::GuiElement(float xPos, float yPos, float xSize, float ySize, int scalingAxis, int alignWith, const std::string& texturePath) : xPos(xPos), yPos(yPos), xSize(xSize), ySize(ySize), scalingAxis(scalingAxis), alignWith(alignWith), texture(GL_NEAREST, GL_REPEAT, texturePath, GL_RGBA8, GL_RGBA, 1.0f, false) {
    /*
     * screen in normalized device coordinates
     * (-1,+1)|-----------------|(+1,+1)
     *        |                 |
     *        |                 |
     * (-1,-1)|-----------------|(+1,-1)
     */
    // TODO put it into quad class with batched rendering
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
    model.translate(xPos, yPos, 0);
    Application::guiManagerPtr->elements.emplace_back(this);
    addKeyBinds();
}

void GuiElement::renderElement(GLShader &shader) {
    shader.uniformMatrix4fv("modelProj", modelProj);
    vao.bind();
    texture.activate(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    vao.unbind();
}

void GuiElement::resize(float width, float height, float oldWidth, float oldHeight, const Matrix4& proj) {
    float xScale = 1, yScale = 1;
    if (scalingAxis != GUI_NO_SCALING) {
        switch (scalingAxis) {
            case GUI_SCALE_NO_AXIS:
                xScale = width/oldWidth;
                yScale = height/oldHeight;
                break;
            case GUI_AXIS_X:
                xScale = width/oldWidth;
                yScale = width/oldWidth;
                break;
            case GUI_AXIS_Y:
                xScale = height/oldHeight;
                yScale = height/oldHeight;
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
    modelProj = proj * model;
}

void GuiElement::addChild(GuiElement* element) {
    if (element && !element->parent && (this->parent != element)) {
        children.emplace_back(element);
        element->parent = this;
    } else {
        LogHelperBE::pushName("GuiElement");
        LogHelperBE::error("Element does already have a parent or is nullptr!");
        LogHelperBE::popName();
    }
}

bool GuiElement::isMouseHover(int mouseX, int mouseY) {
    int guiPosX = (int)std::round(xPos);
    int guiPosY = (int)std::round(yPos);
    int guiSizeX = (int)std::round(xSize);
    int guiSizeY = (int)std::round(ySize);

    if ((mouseX > guiPosX) && (mouseY > guiPosY) && (mouseX < (guiPosX+guiSizeX)) && (mouseY < (guiPosY+guiSizeY))) {
        return true;
    }
    return false;
}

void GuiElement::setPos(float x, float y, const Matrix4& proj) {
    model.translate(x-xPos, y-yPos, 0);
    modelProj = proj * model;
    xPos = x;
    yPos = y;
    for (auto & child : children) {
        setPos(x+child->xPos, y+child->yPos, proj);
    }
}

void GuiElement::setInputListenable(bool value) {
    inputListenable = value;
    for (auto & child : children) {
        setInputListenable(value);
    }
}

bool GuiElement::getInputListenable() const {
    return inputListenable;
}

void GuiElement::setVisible(bool value) {
    isVisible = value;
    for (auto & child : children) {
        setVisible(value);
    }
}

bool GuiElement::getVisible() const {
    return isVisible;
}

GuiElement *GuiElement::getParent() const {
    return parent;
}

GuiElement::~GuiElement() {
    auto it = std::find(Application::guiManagerPtr->elements.begin(), Application::guiManagerPtr->elements.end(), this);
    if (it != Application::guiManagerPtr->elements.end()) {
        Application::guiManagerPtr->elements.erase(it);
    }
}