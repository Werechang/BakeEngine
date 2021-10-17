
#pragma once

#define GUI_NONE 0
#define GUI_TOP_CENTER 1
#define GUI_TOP_RIGHT 2
#define GUI_RIGHT_CENTER 3
#define GUI_BOTTOM_RIGHT 4
#define GUI_BOTTOM_CENTER 5
#define GUI_BOTTOM_LEFT 6
#define GUI_LEFT_CENTER 7
#define GUI_TOP_LEFT 8
#define GUI_CENTER 9

#define GUI_NO_SCALING 0
#define GUI_SCALE_NO_AXIS 1
#define GUI_AXIS_X 2
#define GUI_AXIS_Y 3

#include <vector>
#include "../Render/OpenGL/VertexArray.h"
#include "../Render/OpenGL/GLShader.h"
#include "../Render/OpenGL/GLTexture.h"
#include "../Util/Math/Math.h"
#include "../Input/KeyBind.h"
#include "GLFW/glfw3.h"

class GuiElement {
public:
    GuiElement() : GuiElement(0, 0, 0, 0) {};
    /**
    *
    * @param xPos x Position in pixel coordinates, from left
    * @param yPos y Position in pixel coordinates, from top
    */
    GuiElement(float xPos, float yPos) : GuiElement(xPos, yPos, 0, 0) {};
    /**
    *
    * @param xPos x Position in pixel coordinates, from left
    * @param yPos y Position in pixel coordinates, from top
    * @param xSize x size going from left to right
    * @param ySize y size going from top to bottom
    */
    GuiElement(float xPos, float yPos, float xSize, float ySize) : GuiElement(xPos, yPos, xSize, ySize, GUI_NO_SCALING, GUI_NONE, "textures/gui_placeholder.png") {};
    GuiElement(float xPos, float yPos, float xSize, float ySize, int scalingAxis, int alignWith, const std::string& texturePath);

    void renderElement(GLShader& shader);
    void onResize(float width, float height, float oldWidth, float oldHeight, const Matrix4& proj);
    void addChild(GuiElement* element);

    virtual void onKeyAction(int key, int action) {};
    virtual void onMouseButtonAction(int button, int action) {};
    virtual void onMouseHover() {};
    bool isMouseHover(int mouseX, int mouseY);

    void setPos(float x, float y, const Matrix4& proj);
    void setVisible(bool value);
    bool getVisible() const;
    GuiElement* getParent() const;
private:
    GuiElement* parent = nullptr;
    std::vector<GuiElement*> children;
    VertexArray vao;
    float xPos, yPos, xSize, ySize;
    int scalingAxis;
    int alignWith;
    Matrix4 model = Matrix4::identity();
    Matrix4 modelProj = Matrix4::identity();
    GLTexture texture;
    bool isVisible = true;
};