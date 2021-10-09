
#ifndef BAKEENGINE_KEYBIND_H
#define BAKEENGINE_KEYBIND_H

#define ARRAY_MOUSE_BUTTON 349

#include <map>
#include <utility>
#include <vector>
#include <functional>
#include "InputCallable.h"

static bool KeysAndButtons[357];
// If the keys just were pressed/released
static bool KeysAndButtonsPressed[357];
static bool KeysAndButtonsReleased[357];

class AbstractKeyBindFunction {
public:
    virtual void execute() const {};
};

class KeyBindFunctionStaticNoArgs : public AbstractKeyBindFunction {
public:
    std::function<void()> function;

    explicit KeyBindFunctionStaticNoArgs(std::function<void()> function) : function(std::move(function)) {};

    void execute() const override {
        function.operator()();
    };
};

class KeyBindFunctionObjectNoArgs : public AbstractKeyBindFunction {
public:
    std::function<void(InputCallable&)> function;
    InputCallable* obj;

    KeyBindFunctionObjectNoArgs(std::function<void(InputCallable&)> function, InputCallable* obj) : function(std::move(function)), obj(obj) {};

    void execute() const override {
        function.operator()(*obj);
    };
};

class KeyBind {
private:
    std::map<int, std::unique_ptr<AbstractKeyBindFunction>> modsFunctions;
public:
    static std::map<int, std::unique_ptr<KeyBind>> keyBinds;

    KeyBind() = default;
    KeyBind(int mods, const std::function<void()>& function);
    KeyBind(int mods, const std::function<void(InputCallable &)>& function, InputCallable* obj);

    void checkActive(int mods) const;
    void addFunction(int mods, const std::function<void()>& function);
    void addFunction(int mods, const std::function<void(InputCallable&)>& function, InputCallable* obj);
};


#endif