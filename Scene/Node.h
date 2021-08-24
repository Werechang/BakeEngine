
#ifndef BAKEENGINE_NODE_H
#define BAKEENGINE_NODE_H


#include <vector>
#include "Mesh.h"

class Node {
private:
    std::vector<Node> children;
    std::vector<Mesh*> meshes;
};


#endif //BAKEENGINE_NODE_H
