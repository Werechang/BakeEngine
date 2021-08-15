// Now the door to a new light is opening. That's exciting and inspiring.
#include "InputManager.h"

void InputManager::update() {

}

void InputManager::addAction(int keys[], Function callFun, int numKeys) {
    actions.emplace_back(Action(idPtr, keys, callFun, numKeys));
}