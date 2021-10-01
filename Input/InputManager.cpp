// Now the door to a new light is opening. That's exciting and inspiring.
#include "InputManager.h"

void InputManager::update() const {
    for (auto && node : inputNodesLayer0) {
        if (node->checkActive()) {
            return;
        }
    }
}

bool InputManager::isAnyKeyInTree(std::vector<int>& keys) const {
    // TODO change return type to vector<node*> or something different
    for (auto & node : inputNodes) {
        for (auto key : keys) {
            if (node.getKey() == key) {
                return true;
            }
        }
    }
    return false;
}

void InputManager::addAction(std::function<void()> &function, int key1) {
    // TODO clean up the mess, this is just temporary to get an overview
    // maybe cache action keys + order in layers
    // TODO Change logic completely?
    std::vector<int> keys = {key1};
    if (!isAnyKeyInTree(keys)) {
        inputNodes.emplace_back(InputNode(key1));
        InputNode* node = &inputNodes.back();
        inputNodesLayer0.emplace_back(node);
        node->setFunction(&function);
    } else {
        for (auto & node : inputNodes) {
            if (node.getKey() == key1) {
                if (!node.getParent()) {
                    node.setFunction(&function);
                    return;
                } else {
                    // check if parents dont have other functions/child nodes
                    InputNode* index = node.getParent();
                    while (index) {
                        if (index->hasFunction() && !index->singleChild(keys)) {
                            // if parents have other children/functions, the action gets a new node
                            inputNodes.emplace_back(InputNode(key1));
                            InputNode* newNode = &inputNodes.back();
                            inputNodesLayer0.emplace_back(newNode);
                            newNode->setFunction(&function);
                            return;
                        }
                        index = index->getParent();
                    }
                    // !!!IMPORTANT!!!
                    // case parents can change position. note that the node we want to place at layer0 may have
                    // functions which should be added to the parent which will be placed in the lowest layer.
                }
            }
        }
    }
}

void InputManager::addAction(std::function<void()> &function, int key1, int key2) {
}

void InputManager::addAction(std::function<void()> &function, int key1, int key2, int key3) {
}

void InputManager::addAction(std::function<void()> &function, int key1, int key2, int key3, int key4) {
}