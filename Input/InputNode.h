
#ifndef BAKEENGINE_INPUTNODE_H
#define BAKEENGINE_INPUTNODE_H

#include <map>
#include <functional>
#include "InputCallable.h"

static bool KeyboardKeys[349];
static bool MouseButtons[8];
static bool MouseButtonWasJustPressed[8];

class AbstractNodeFunction {
public:
    virtual void execute() const {};
};

class NodeFunctionStaticNoArgs : public AbstractNodeFunction {
public:
    std::function<void()>* function;

    explicit NodeFunctionStaticNoArgs(std::function<void()>* function) : function(function) {};

    void execute() const override {
        function->operator()();
    };};

class NodeFunctionObjectNoArgs : public AbstractNodeFunction {
public:
    std::function<void(InputCallable&)>* function;
    InputCallable* obj;

    NodeFunctionObjectNoArgs(std::function<void(InputCallable&)>* function, InputCallable* obj) : function(function), obj(obj) {

    };

    void execute() const override {
        function->operator()(*obj);
    };
};

class InputNode {
private:
    std::weak_ptr<AbstractNodeFunction> nodeFunction;
    int key;
    std::vector<InputNode*> children;
    InputNode* parent = nullptr;
public:
    explicit InputNode(int key);

    bool checkActive() const;
    void setFunction(std::function<void()>* function);
    void setFunction(std::function<void(InputCallable&)>* function, InputCallable* obj);
    void addChild(InputNode* node);
    int getKey() const;
    InputNode* getParent() const;
    bool hasFunction() const;
    bool singleChild(std::vector<int>& keys) const;
};


#endif