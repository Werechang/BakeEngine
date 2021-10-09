// Now the door to a new light is opening. That's exciting and inspiring.
#include "InputManager.h"

void InputManager::addAction(const std::function<void()>& function, int key, int mods) {
    try {
        auto & bind = KeyBind::keyBinds.at(key);
        bind->addFunction(mods, function);
    } catch (std::out_of_range&) {
        KeyBind::keyBinds.insert(std::make_pair(key, std::make_unique<KeyBind>(mods, function)));
    }
}

void InputManager::addAction(const std::function<void(InputCallable&)>& function, InputCallable *obj, int key, int mods) {
    try {
        auto & bind = KeyBind::keyBinds.at(key);
        bind->addFunction(mods, function, obj);
    } catch (std::out_of_range&) {
        KeyBind::keyBinds.insert(std::make_pair(key, std::make_unique<KeyBind>(mods, function, obj)));
    }
}