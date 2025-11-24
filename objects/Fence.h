#ifndef FENCE_H
#define FENCE_H

#include "Mesh.h"
#include "../Shader.h"
#include <glm/glm.hpp>
#include <vector>

/**
 * Decorative fence with traditional square patterns
 * Like the real fence at Ba Dinh Square
 */
class Fence
{
public:
    std::vector<Mesh *> segments;
    std::vector<Mesh *> patterns;
    
    glm::vec3 startPos;
    glm::vec3 endPos;
    float height;
    
    Fence(glm::vec3 start, glm::vec3 end, float h = 1.0f);
    ~Fence();
    
    void createFenceSegments();
    void createSquarePatterns();
    
    void draw(Shader &shader);
};

#endif
