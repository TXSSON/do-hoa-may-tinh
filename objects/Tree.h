#ifndef TREE_H
#define TREE_H

#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "../Shader.h"
#include "../models/Texture.h"

/**
 * Realistic tree model with trunk and spreading foliage
 * For decorative landscaping around Lang Bac
 */
class Tree
{
public:
    Mesh *trunk;
    std::vector<Mesh *> branches;
    std::vector<glm::mat4> branchTransforms;
    
    std::vector<Mesh *> foliageParts;
    std::vector<glm::mat4> foliageTransforms; // Changed from offsets to full transforms for rotation
    
    glm::vec3 position;
    float scale;
    
    Tree(glm::vec3 pos, float treeScale = 1.0f);
    ~Tree();
    
    void createBranch(glm::vec3 startPos, glm::vec3 direction, float length, float radius, int depth);
    void draw(Shader &shader, Texture *barkTex, Texture *leafTex);
    
    // Deprecated but kept for compatibility if needed (though draw() is preferred)
    glm::mat4 getTrunkTransform() const;
};

#endif
