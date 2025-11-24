#ifndef COTCO_H
#define COTCO_H

#include "Mesh.h"
#include <glm/glm.hpp>

/**
 * Cột cờ (Flag Pole) với lá cờ Việt Nam
 * V2.0-Beta: With animation support
 */
class CotCo
{
public:
    Mesh *pole;     // Cylinder pole
    Mesh *flag;     // Plane for flag
    Mesh *base;     // Base platform
    glm::vec3 position;

    CotCo(glm::vec3 pos);
    ~CotCo();

    // V2.0-Beta: Animation
    void update(float deltaTime);
    glm::vec3 getFlagPosition(); // For flag at current height
    glm::mat4 getFlagTransform(); // With wave animation
    
    void raiseFlag();  // Start raising animation
    void lowerFlag();  // Start lowering animation

private:
    // Animation state
    float waveTime;        // For sine wave motion
    float flagHeight;      // 0.0 (lowered) to 1.0 (raised)
    bool isRaising;
    bool isLowering;
    float animationSpeed;  // Height change rate
};

#endif
