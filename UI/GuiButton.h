//
// Created by tom on 08.10.2021.
//

#ifndef BAKEENGINE_GUIBUTTON_H
#define BAKEENGINE_GUIBUTTON_H


#include "GuiElement.h"

class GuiButton : public GuiElement{
private:
    bool isActive;
public:
    GuiButton(float xPos, float yPos, float xSize, float ySize, int scalingAxis, int alignWith) : GuiElement(xPos, yPos, xSize, ySize, scalingAxis, alignWith, "textures/gui_placeholder.png") {};

    void onMouseButtonAction(int button, int action) override {
        if (KeysAndButtonsPressed[ARRAY_MOUSE_BUTTON + GLFW_MOUSE_BUTTON_LEFT]) {
            onActivate();
        } else if (KeysAndButtonsReleased[ARRAY_MOUSE_BUTTON + GLFW_MOUSE_BUTTON_LEFT]) {
            onDeactivate();
        }
    };
    void onActivate() {
        isActive = true;
    };
    void onDeactivate() {
        isActive = false;
    };
    bool getActive() {
        return isActive;
    };
};


#endif //BAKEENGINE_GUIBUTTON_H
