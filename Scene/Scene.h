
#ifndef BAKEENGINE_SCENE_H
#define BAKEENGINE_SCENE_H


#include "Node.h"
#include "Material.h"

class Scene {
private:
    Node rootNode;
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
public:

};


#endif //BAKEENGINE_SCENE_H
