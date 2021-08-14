// Now the door to a new light is opening. That's exciting and inspiring.#include "InputManager.h"
#include "InputManager.h"

void InputManager::update() {

}

void InputManager::addAction(int* keys, Function callFun) {
    actions.emplace_back(idPtr++, keys, callFun);
}