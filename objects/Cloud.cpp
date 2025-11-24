#include "Cloud.h"
#include "Primitives.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

Cloud::Cloud(glm::vec3 startPos, float spd, float sc)
    : position(startPos), driftSpeed(spd), baseScale(sc)
{
    altitude = startPos.y;
    speed = spd;
    
    // Random rotation and movement parameters
    rotation = static_cast<float>(rand()) / RAND_MAX * 360.0f;
    rotationSpeed = 2.0f + (rand() % 5); // 2-6 degrees per second
    
    // Bobbing parameters (vertical oscillation)
    bobAmplitude = 2.0f + (rand() % 3); // 2-4 units
    bobFrequency = 0.3f + (rand() % 10) / 20.0f; // 0.3-0.8 Hz
    bobPhase = static_cast<float>(rand()) / RAND_MAX * 6.28f; // Random starting phase
    
    // Create volumetric cloud shape
    createCloudShape();
}

Cloud::~Cloud()
{
    // Clean up all sphere meshes
    for (auto sphere : spheres)
    {
        if (sphere) delete sphere;
    }
    spheres.clear();
}

void Cloud::createCloudShape()
{
    // Create 4-6 ellipsoids to form a wispy, streak-like cloud (cirrus style)
    int numSpheres = 4 + (rand() % 3); // 4-6 ellipsoids
    
    for (int i = 0; i < numSpheres; i++)
    {
        // Create sphere mesh (unit sphere, will be stretched into ellipsoid)
        Mesh* sphere = Primitives::createSphere(1.0f, 16, 16);
        spheres.push_back(sphere);
        
        // Arrange in a LINE to create streak effect
        // Main axis along X (horizontal streak)
        float offsetX = ((float)i / numSpheres - 0.5f) * 2.0f; // Spread along X: -1 to 1
        float offsetY = ((rand() % 100) / 500.0f - 0.1f) * 0.3f; // Small Y variation: -0.06 to 0.06
        float offsetZ = ((rand() % 100) / 500.0f - 0.1f) * 0.2f; // Small Z variation: -0.04 to 0.04
        
        sphereOffsets.push_back(glm::vec3(offsetX, offsetY, offsetZ));
        
        // Individual sphere scale (slight variation)
        float sphereScale = 0.6f + (rand() % 40) / 100.0f; // 0.6 to 1.0
        sphereScales.push_back(sphereScale);
    }
}

void Cloud::update(float deltaTime)
{
    // Horizontal drift
    position.x += driftSpeed * deltaTime;
    
    // Vertical bobbing (sine wave)
    bobPhase += bobFrequency * deltaTime;
    float bobOffset = sin(bobPhase) * bobAmplitude;
    position.y = altitude + bobOffset;
    
    // Slow rotation
    rotation += rotationSpeed * deltaTime;
    if (rotation > 360.0f) rotation -= 360.0f;
    
    // Check wrapping bounds (wider range for horizon coverage)
    checkBounds(-2000.0f, 2000.0f);
}

glm::mat4 Cloud::getTransform(int sphereIndex) const
{
    if (sphereIndex < 0 || sphereIndex >= spheres.size())
        return glm::mat4(1.0f);
    
    glm::mat4 transform = glm::mat4(1.0f);
    
    // Translate to cloud position
    transform = glm::translate(transform, position);
    
    // Apply rotation around Y axis
    transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Apply sphere offset
    transform = glm::translate(transform, sphereOffsets[sphereIndex] * baseScale);
    
    // Apply ELONGATED scale to create wispy streaks (ellipsoid)
    // Make it MUCH wider (X) and longer (Z) than tall (Y)
    float finalScale = baseScale * sphereScales[sphereIndex];
    float stretchX = finalScale * 3.5f; // Very wide
    float stretchY = finalScale * 0.3f; // Very thin (wispy)
    float stretchZ = finalScale * 1.2f; // Moderate depth
    
    transform = glm::scale(transform, glm::vec3(stretchX, stretchY, stretchZ));
    
    return transform;
}

void Cloud::checkBounds(float minX, float maxX)
{
    // Wrap around when cloud goes off screen
    if (position.x > maxX)
    {
        position.x = minX;
    }
    else if (position.x < minX)
    {
        position.x = maxX;
    }
}

