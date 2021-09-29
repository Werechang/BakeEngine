// Now the door to a new light is opening. That's exciting and inspiring.
#include "InputManager.h"

void InputManager::update() {
    for (auto & pair : inputNodes) {
        if (pair.second == 0)
            pair.first->checkActive();
    }
}

void InputManager::resortNodeTree() {
    //TODO Complicated calculations and checks
}

void InputManager::addAction(std::function<void()> &function, int key1) {
}

void InputManager::addAction(std::function<void()> &function, int key1, int key2) {
}

void InputManager::addAction(std::function<void()> &function, int key1, int key2, int key3) {
}

void InputManager::addAction(std::function<void()> &function, int key1, int key2, int key3, int key4) {
}

InputNode *InputManager::findKeyLayer0(int key) {
    for (auto & pair : inputNodes) {
        if ((pair.second == 0) && (pair.first->getKey() == key))
            return pair.first.get();
    }
    return nullptr;
}
