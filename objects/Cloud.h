#ifndef CLOUD_H
#define CLOUD_H

#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

/**
 * Animated cloud - moves across the sky
 * V3.0: Volumetric 3D cloud made of multiple spheres with complex movement
 */
class Cloud
{
public:
    // Volumetric cloud structure
    std::vector<Mesh*> spheres;           // Multiple spheres forming cloud shape
    std::vector<glm::vec3> sphereOffsets; // Relative positions of spheres
    std::vector<float> sphereScales;      // Individual sphere scales
    
    float baseScale;      // Overall cloud scale
    float rotation;       // Rotation angle around Y axis
    
    glm::vec3 position;   // Center position
    float speed;          // Horizontal drift speed
    float altitude;       // Y position (height)
    
    // Complex movement
    float driftSpeed;     // Speed along X axis
    float bobAmplitude;   // Vertical oscillation amplitude
    float bobFrequency;   // Vertical oscillation frequency
    float bobPhase;       // Current phase of bobbing
    float rotationSpeed;  // Rotation speed around Y axis

    Cloud(glm::vec3 startPos, float spd, float scale);
    ~Cloud();

    void update(float deltaTime);
    glm::vec3 getPosition() const { return position; }
    
    // Get transform for individual sphere
    glm::mat4 getTransform(int sphereIndex) const;
    
    // Get number of spheres
    int getSphereCount() const { return spheres.size(); }
    
    // Wrap around screen edges
    void checkBounds(float minX, float maxX);

private:
    void createCloudShape(); // Generate random volumetric cloud shape
};

#endif

