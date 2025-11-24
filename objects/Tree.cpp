#include "Tree.h"
#include "Primitives.h"
#include <cmath>
#include <glm/gtx/vector_angle.hpp>

Tree::Tree(glm::vec3 pos, float treeScale)
    : position(pos), scale(treeScale)
{
    // Trunk - brown cylinder
    float trunkHeight = 3.0f * scale;
    float trunkRadius = 0.25f * scale;
    trunk = Primitives::createCylinder(trunkRadius, trunkHeight, 12);
    
    // Create branching structure
    // Start 3 main branches from top of trunk
    glm::vec3 trunkTop = glm::vec3(0.0f, trunkHeight, 0.0f);
    
    // Branch 1
    createBranch(trunkTop, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)), 2.5f * scale, trunkRadius * 0.8f, 2);
    // Branch 2
    createBranch(trunkTop, glm::normalize(glm::vec3(-0.5f, 1.0f, 0.866f)), 2.5f * scale, trunkRadius * 0.8f, 2);
    // Branch 3
    createBranch(trunkTop, glm::normalize(glm::vec3(-0.5f, 1.0f, -0.866f)), 2.5f * scale, trunkRadius * 0.8f, 2);
    // Branch 4 (Central vertical extension)
    createBranch(trunkTop, glm::vec3(0.0f, 1.0f, 0.0f), 2.0f * scale, trunkRadius * 0.9f, 2);
}

Tree::~Tree()
{
    delete trunk;
    for (auto branch : branches) delete branch;
    branches.clear();
    for (auto part : foliageParts) delete part;
    foliageParts.clear();
}

void Tree::createBranch(glm::vec3 startPos, glm::vec3 direction, float length, float radius, int depth)
{
    // Create branch mesh (cylinder)
    // Cylinder is created along Y axis, centered at origin.
    // We need to scale it to length/radius, rotate it to direction, and translate to startPos + length/2 * direction
    
    Mesh *branchMesh = Primitives::createCylinder(radius, length, 8);
    branches.push_back(branchMesh);
    
    // Calculate transform
    glm::vec3 centerPos = startPos + direction * (length * 0.5f);
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, centerPos);
    
    // Rotation
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    if (glm::abs(glm::dot(up, direction)) < 0.99f)
    {
        glm::vec3 axis = glm::cross(up, direction);
        float angle = glm::acos(glm::dot(up, direction));
        transform = glm::rotate(transform, angle, axis);
    }
    
    branchTransforms.push_back(transform);
    
    // Recursion or Foliage
    glm::vec3 endPos = startPos + direction * length;
    
    if (depth > 0)
    {
        // Spawn 2 sub-branches
        // Rotate direction slightly
        // Simple way: add random/fixed vector to direction and normalize
        
        // Sub-branch 1
        glm::vec3 dir1 = glm::normalize(direction + glm::vec3(0.5f, 0.0f, 0.5f));
        createBranch(endPos, dir1, length * 0.7f, radius * 0.7f, depth - 1);
        
        // Sub-branch 2
        glm::vec3 dir2 = glm::normalize(direction + glm::vec3(-0.5f, 0.0f, -0.5f));
        createBranch(endPos, dir2, length * 0.7f, radius * 0.7f, depth - 1);
    }
    else
    {
        // Add foliage cluster at end
        // Create a few small spheres
        for (int i = 0; i < 3; i++)
        {
            Mesh *leaf = Primitives::createSphere(0.8f * scale, 8, 8);
            foliageParts.push_back(leaf);
            
            glm::mat4 leafTransform = glm::mat4(1.0f);
            // Randomize position slightly around endPos
            float ox = (i == 0) ? 0.0f : (i == 1 ? 0.5f : -0.5f);
            float oz = (i == 0) ? 0.0f : (i == 1 ? 0.5f : -0.5f);
            float oy = (i == 0) ? 0.5f : 0.0f;
            
            leafTransform = glm::translate(leafTransform, endPos + glm::vec3(ox, oy, oz) * scale);
            foliageTransforms.push_back(leafTransform);
        }
    }
}

void Tree::draw(Shader &shader, Texture *barkTex, Texture *leafTex)
{
    // Render Trunk
    if (barkTex) barkTex->bind(0);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f * scale, 0.0f)); // Center of trunk (height 3.0)
    shader.setMat4("model", model);
    trunk->draw();
    
    // Render Branches
    for (size_t i = 0; i < branches.size(); i++)
    {
        // Branch transform is relative to tree origin (0,0,0)
        // So we apply tree position first
        model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = model * branchTransforms[i];
        shader.setMat4("model", model);
        branches[i]->draw();
    }
    
    // Render Foliage
    if (leafTex) leafTex->bind(0);
    for (size_t i = 0; i < foliageParts.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = model * foliageTransforms[i];
        shader.setMat4("model", model);
        foliageParts[i]->draw();
    }
}

glm::mat4 Tree::getTrunkTransform() const
{
    return glm::mat4(1.0f); // Deprecated
}
