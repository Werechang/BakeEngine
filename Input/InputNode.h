
#ifndef BAKEENGINE_INPUTNODE_H
#define BAKEENGINE_INPUTNODE_H

#include <map>
#include <functional>
#include "InputCallable.h"

static bool KeyboardKeys[349];
static bool MouseButtons[8];
static bool MouseButtonWasJustPressed[8];

class InputNode {
private:
    std::vector<std::function<void()>*> staticFunctions;
    std::map<std::function<void(InputCallable&)>*, InputCallable*> nonStaticFunctions;
    int key;
    std::vector<InputNode*> children;
    InputNode* parent = nullptr;
public:
    explicit InputNode(int key);
    void checkActive() const;
    // TODO Templates with this
    void addStaticFunction(std::function<void()>* function);
    void addNonStaticFunction(std::function<void(InputCallable&)>* function, InputCallable* obj);
    void addChild(InputNode* node);
    int getKey() const;
};


#endif