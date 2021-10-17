// Now the door to a new light is opening. That's exciting and inspiring.
#include "InputManager.h"

/**
 * Sets a function to get called if a specific key gets pressed/released/repeated (updates every frame)
 * @param function Pointer to function. Use std::bind or lambda (example: [this] {return function(param1, param2)}) to use function with arguments and non-static member functions.
 * @param key A specific key on which the function gets called. Use the definitions provided by GLFW. Example: GLFW_KEY_ESCAPE
 * @param mods Keys which need te be pressed to activate this KeyBind. Mods are GLFW_MOD_CONTROL, GLFW_MOD_ALT and GLFW_MOD_SHIFT. You do not need to specify one (pass 0 as a param). If you want to have multiple mods use the bitwise or | operator.
 * @param onAction The action on which the KeyBind is activated. Actions are INPUT_ON_PRESS, INPUT_ON_ACTIVE (on press and repeat) and INPUT_ON_RELEASE
 */
void InputManager::addKeyBind(const std::function<void()>& function, int key, int mods, int onAction) {
    if (((mods & GLFW_MOD_SUPER) == GLFW_MOD_SUPER) || ((mods & GLFW_MOD_CAPS_LOCK) == GLFW_MOD_CAPS_LOCK) || ((mods & GLFW_MOD_NUM_LOCK) == GLFW_MOD_NUM_LOCK)) {
        LogHelperBE::pushName("InputManager");
        LogHelperBE::error("This mod is not supported");
        LogHelperBE::popName();
    }
    switch (key) {
        case GLFW_KEY_LEFT_CONTROL:
            case GLFW_KEY_RIGHT_CONTROL:
                case GLFW_KEY_LEFT_SHIFT:
                    case GLFW_KEY_RIGHT_SHIFT:
                        case GLFW_KEY_LEFT_ALT:
                            case GLFW_KEY_RIGHT_ALT:
                                case GLFW_KEY_LEFT_SUPER:
                                    case GLFW_KEY_RIGHT_SUPER:
                                        case GLFW_KEY_CAPS_LOCK:
                                            case GLFW_KEY_NUM_LOCK:
                                                LogHelperBE::pushName("InputManager");
                                                LogHelperBE::error("Key " + std::to_string(key) + " is a mod key");
                                                LogHelperBE::popName();
                                                break;
                                                default:
                                                    switch (onAction) {
                                                        case INPUT_ON_PRESS: {
                                                            if (keyBindsOnPress.find(key) != keyBindsOnPress.end()) {
                                                                keyBindsOnPress[key]->addFunction(mods ,function);
                                                                break;
                                                            } else {
                                                                keyBindsOnPress.insert(std::make_pair(key, std::make_unique<KeyBind>(mods, function)));
                                                            }
                                                        }
                                                        case INPUT_ON_ACTIVE: {
                                                            if (keyBindsOnActive.find(key) != keyBindsOnActive.end()) {
                                                                keyBindsOnActive[key]->addFunction(mods ,function);
                                                                break;
                                                            } else {
                                                                keyBindsOnActive.insert(std::make_pair(key, std::make_unique<KeyBind>(mods, function)));
                                                            }
                                                        }
                                                        case INPUT_ON_RELEASE: {
                                                            if (keyBindsOnRelease.find(key) != keyBindsOnRelease.end()) {
                                                                keyBindsOnRelease[key]->addFunction(mods ,function);
                                                                break;
                                                            } else {
                                                                keyBindsOnRelease.insert(std::make_pair(key, std::make_unique<KeyBind>(mods, function)));
                                                            }
                                                        }
                                                    }
    }
}

void InputManager::updateInput(GLFWwindow* window) {
    for (auto & pair : activeKeys) {
        // pair.second : if this is not in the same frame as this action was added (to prevent double inputs)
        if (pair.second) {
            if (glfwGetKey(window, pair.first) == GLFW_PRESS) {
                keyBindsOnActive[pair.first]->checkActive(activeMods);
            }
        } else {
            pair.second = true;
        }
    }
    // TODO Fix mouse input
    /*
    if (KeysAndButtons[ARRAY_MOUSE_BUTTON + GLFW_MOUSE_BUTTON_LEFT]) {
        if (KeysAndButtonsPressed[ARRAY_MOUSE_BUTTON + GLFW_MOUSE_BUTTON_LEFT]) {
            firstMouse = true;
        } else if (!firstMouse) {
            float xOffset = (float)(mouseX - lastMouseX) * 0.1f;
            float yOffset = (float)(lastMouseY - mouseY) * 0.1f;
            camera.turn(xOffset, yOffset);
            lastMouseX = mouseX;
            lastMouseY = mouseY;
        }
    }*/
}

void InputManager::updateKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_UNKNOWN) return;
    else if (action == GLFW_PRESS) {
        if (keyBindsOnPress.find(key) != keyBindsOnPress.end()) {
            keyBindsOnPress[key]->checkActive(mods);
        }
        if (keyBindsOnActive.find(key) != keyBindsOnActive.end()) {
            keyBindsOnActive[key]->checkActive(mods);
            // If this key is not already in the vector. Put it only in there if this is a keybind
            if (activeKeys.find(key) == activeKeys.end()) {
                activeKeys.insert(std::make_pair(key, false));
            }
        }
        if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
            activeMods += GLFW_MOD_CONTROL;
        } else if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
            activeMods += GLFW_MOD_SHIFT;
        } else if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) {
            activeMods += GLFW_MOD_ALT;
        }
    } else if (action == GLFW_RELEASE) {
        if (keyBindsOnRelease.find(key) != keyBindsOnRelease.end()) {
            keyBindsOnRelease[key]->checkActive(mods);
            auto it = activeKeys.find(key);
            if (it != activeKeys.end()) {
                activeKeys.erase(it);
            }
        }
        if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
            activeMods -= GLFW_MOD_CONTROL;
        } else if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
            activeMods -= GLFW_MOD_SHIFT;
        } else if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) {
            activeMods -= GLFW_MOD_ALT;
        }
    } /*else if (action == GLFW_REPEAT) {
        if (keyBindsOnActive.find(key) != keyBindsOnActive.end()) {
            keyBindsOnActive[key]->checkActive(mods);
        }
    }*/
}

void InputManager::updateMouse(GLFWwindow *window, double xPos, double yPos) {
    if (firstMouse) {
        lastMouseX = xPos;
        lastMouseY = yPos;
        firstMouse = false;
    }
    mouseX = xPos;
    mouseY = yPos;
    int screenX = (int)std::round(mouseX);
    int screenY = (int)std::round(mouseY);
    // TODO Fix input
    /*for (auto & guiElement : GuiElement::guiElements) {
        if (guiElement->getVisible()) {
            if (guiElement->isMouseHover(screenX, screenY)) {
                onMouseHoverGuiElement = guiElement;
                return;
            }
        }
    }*/
}

void InputManager::updateMouseButton(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        mouseButtons[button] = true;
        // TODO set a variable to true which says that the app should update the mouse buttons on the next frame (for GLFW_REPEAT)
    }
    else if (action == GLFW_RELEASE) {
        mouseButtons[button] = false;
    }
}

const char *InputManager::getKeyName(int key) {
    switch (key) {
        // Printable keys
        case GLFW_KEY_A:            return "A";
        case GLFW_KEY_B:            return "B";
        case GLFW_KEY_C:            return "C";
        case GLFW_KEY_D:            return "D";
        case GLFW_KEY_E:            return "E";
        case GLFW_KEY_F:            return "F";
        case GLFW_KEY_G:            return "G";
        case GLFW_KEY_H:            return "H";
        case GLFW_KEY_I:            return "I";
        case GLFW_KEY_J:            return "J";
        case GLFW_KEY_K:            return "K";
        case GLFW_KEY_L:            return "L";
        case GLFW_KEY_M:            return "M";
        case GLFW_KEY_N:            return "N";
        case GLFW_KEY_O:            return "O";
        case GLFW_KEY_P:            return "P";
        case GLFW_KEY_Q:            return "Q";
        case GLFW_KEY_R:            return "R";
        case GLFW_KEY_S:            return "S";
        case GLFW_KEY_T:            return "T";
        case GLFW_KEY_U:            return "U";
        case GLFW_KEY_V:            return "V";
        case GLFW_KEY_W:            return "W";
        case GLFW_KEY_X:            return "X";
        case GLFW_KEY_Y:            return "Y";
        case GLFW_KEY_Z:            return "Z";
        case GLFW_KEY_1:            return "1";
        case GLFW_KEY_2:            return "2";
        case GLFW_KEY_3:            return "3";
        case GLFW_KEY_4:            return "4";
        case GLFW_KEY_5:            return "5";
        case GLFW_KEY_6:            return "6";
        case GLFW_KEY_7:            return "7";
        case GLFW_KEY_8:            return "8";
        case GLFW_KEY_9:            return "9";
        case GLFW_KEY_0:            return "0";
        case GLFW_KEY_SPACE:        return "SPACE";
        case GLFW_KEY_MINUS:        return "MINUS";
        case GLFW_KEY_EQUAL:        return "EQUAL";
        case GLFW_KEY_LEFT_BRACKET: return "LEFT BRACKET";
        case GLFW_KEY_RIGHT_BRACKET: return "RIGHT BRACKET";
        case GLFW_KEY_BACKSLASH:    return "BACKSLASH";
        case GLFW_KEY_SEMICOLON:    return "SEMICOLON";
        case GLFW_KEY_APOSTROPHE:   return "APOSTROPHE";
        case GLFW_KEY_GRAVE_ACCENT: return "GRAVE ACCENT";
        case GLFW_KEY_COMMA:        return "COMMA";
        case GLFW_KEY_PERIOD:       return "PERIOD";
        case GLFW_KEY_SLASH:        return "SLASH";
        case GLFW_KEY_WORLD_1:      return "WORLD 1";
        case GLFW_KEY_WORLD_2:      return "WORLD 2";

        // Function keys
        case GLFW_KEY_ESCAPE:       return "ESCAPE";
        case GLFW_KEY_F1:           return "F1";
        case GLFW_KEY_F2:           return "F2";
        case GLFW_KEY_F3:           return "F3";
        case GLFW_KEY_F4:           return "F4";
        case GLFW_KEY_F5:           return "F5";
        case GLFW_KEY_F6:           return "F6";
        case GLFW_KEY_F7:           return "F7";
        case GLFW_KEY_F8:           return "F8";
        case GLFW_KEY_F9:           return "F9";
        case GLFW_KEY_F10:          return "F10";
        case GLFW_KEY_F11:          return "F11";
        case GLFW_KEY_F12:          return "F12";
        case GLFW_KEY_F13:          return "F13";
        case GLFW_KEY_F14:          return "F14";
        case GLFW_KEY_F15:          return "F15";
        case GLFW_KEY_F16:          return "F16";
        case GLFW_KEY_F17:          return "F17";
        case GLFW_KEY_F18:          return "F18";
        case GLFW_KEY_F19:          return "F19";
        case GLFW_KEY_F20:          return "F20";
        case GLFW_KEY_F21:          return "F21";
        case GLFW_KEY_F22:          return "F22";
        case GLFW_KEY_F23:          return "F23";
        case GLFW_KEY_F24:          return "F24";
        case GLFW_KEY_F25:          return "F25";
        case GLFW_KEY_UP:           return "UP";
        case GLFW_KEY_DOWN:         return "DOWN";
        case GLFW_KEY_LEFT:         return "LEFT";
        case GLFW_KEY_RIGHT:        return "RIGHT";
        case GLFW_KEY_LEFT_SHIFT:   return "LEFT SHIFT";
        case GLFW_KEY_RIGHT_SHIFT:  return "RIGHT SHIFT";
        case GLFW_KEY_LEFT_CONTROL: return "LEFT CONTROL";
        case GLFW_KEY_RIGHT_CONTROL: return "RIGHT CONTROL";
        case GLFW_KEY_LEFT_ALT:     return "LEFT ALT";
        case GLFW_KEY_RIGHT_ALT:    return "RIGHT ALT";
        case GLFW_KEY_TAB:          return "TAB";
        case GLFW_KEY_ENTER:        return "ENTER";
        case GLFW_KEY_BACKSPACE:    return "BACKSPACE";
        case GLFW_KEY_INSERT:       return "INSERT";
        case GLFW_KEY_DELETE:       return "DELETE";
        case GLFW_KEY_PAGE_UP:      return "PAGE UP";
        case GLFW_KEY_PAGE_DOWN:    return "PAGE DOWN";
        case GLFW_KEY_HOME:         return "HOME";
        case GLFW_KEY_END:          return "END";
        case GLFW_KEY_KP_0:         return "KEYPAD 0";
        case GLFW_KEY_KP_1:         return "KEYPAD 1";
        case GLFW_KEY_KP_2:         return "KEYPAD 2";
        case GLFW_KEY_KP_3:         return "KEYPAD 3";
        case GLFW_KEY_KP_4:         return "KEYPAD 4";
        case GLFW_KEY_KP_5:         return "KEYPAD 5";
        case GLFW_KEY_KP_6:         return "KEYPAD 6";
        case GLFW_KEY_KP_7:         return "KEYPAD 7";
        case GLFW_KEY_KP_8:         return "KEYPAD 8";
        case GLFW_KEY_KP_9:         return "KEYPAD 9";
        case GLFW_KEY_KP_DIVIDE:    return "KEYPAD DIVIDE";
        case GLFW_KEY_KP_MULTIPLY:  return "KEYPAD MULTIPLY";
        case GLFW_KEY_KP_SUBTRACT:  return "KEYPAD SUBTRACT";
        case GLFW_KEY_KP_ADD:       return "KEYPAD ADD";
        case GLFW_KEY_KP_DECIMAL:   return "KEYPAD DECIMAL";
        case GLFW_KEY_KP_EQUAL:     return "KEYPAD EQUAL";
        case GLFW_KEY_KP_ENTER:     return "KEYPAD ENTER";
        case GLFW_KEY_PRINT_SCREEN: return "PRINT SCREEN";
        case GLFW_KEY_NUM_LOCK:     return "NUM LOCK";
        case GLFW_KEY_CAPS_LOCK:    return "CAPS LOCK";
        case GLFW_KEY_SCROLL_LOCK:  return "SCROLL LOCK";
        case GLFW_KEY_PAUSE:        return "PAUSE";
        case GLFW_KEY_LEFT_SUPER:   return "LEFT SUPER";
        case GLFW_KEY_RIGHT_SUPER:  return "RIGHT SUPER";
        case GLFW_KEY_MENU:         return "MENU";

        default:                    return "UNKNOWN";
    }
}

const char *InputManager::getActionName(int action) {
    switch (action) {
        case GLFW_PRESS:
            return "pressed";
            case GLFW_RELEASE:
                return "released";
                case GLFW_REPEAT:
                    return "repeated";
    }

    return "caused unknown action";
}

const char *InputManager::getButtonName(int button) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            return "left";
            case GLFW_MOUSE_BUTTON_RIGHT:
                return "right";
                case GLFW_MOUSE_BUTTON_MIDDLE:
                    return "middle";
                    default:
                        return "Other";
    }
}