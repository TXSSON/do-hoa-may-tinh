#include "CotCo.h"
#include "Primitives.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

/**
 * Flag pole - V2.0-Beta with animations
 */

CotCo::CotCo(glm::vec3 pos) 
    : position(pos), waveTime(0.0f), flagHeight(1.0f), 
      isRaising(false), isLowering(false), animationSpeed(0.1f)
{
    // Base platform
    base = Primitives::createBox(2.0f, 0.5f, 2.0f);

    // Tall pole (white cylinder)
    float poleHeight = 25.0f;
    float poleRadius = 0.2f;
    pole = Primitives::createCylinder(poleRadius, poleHeight, 16);

    // Flag (rectangular plane)
    flag = Primitives::createPlane(3.0f, 2.0f); // 3:2 ratio
}

CotCo::~CotCo()
{
    delete pole;
    delete flag;
    delete base;
}

void CotCo::update(float deltaTime)
{
    // Waving animation - continuous
    waveTime += deltaTime * 2.0f; // Wave frequency
    
    // Raising/Lowering animation
    if (isRaising && flagHeight < 1.0f)
    {
        flagHeight += animationSpeed * deltaTime;
        if (flagHeight >= 1.0f)
        {
            flagHeight = 1.0f;
            isRaising = false;
        }
    }
    else if (isLowering && flagHeight > 0.0f)
    {
        flagHeight -= animationSpeed * deltaTime;
        if (flagHeight <= 0.0f)
        {
            flagHeight = 0.0f;
            isLowering = false;
        }
    }
}

glm::vec3 CotCo::getFlagPosition()
{
    // Flag height based on animation state
    float currentHeight = flagHeight * 25.0f; // 0 to 25 meters
    return position + glm::vec3(0.0f, currentHeight, 0.0f);
}

glm::mat4 CotCo::getFlagTransform()
{
    // Base transform at current height
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, getFlagPosition());
    
    // Position flag so left edge is at pole
    // Flag is 3.0 wide x 2.0 tall, so offset by half width (1.5)
    transform = glm::translate(transform, glm::vec3(1.5f, -1.0f, 0.0f));
    
    // Rotate 90 degrees around X to make flag hang vertically (plane is horizontal by default)
    transform = glm::rotate(transform, glm::radians(-90.0f), glm::vec3(1, 0, 0));
    
    // Wave animation - gentle horizontal wave (like wind blowing)
    float waveAngle = std::sin(waveTime) * 3.0f; // ±3 degrees
    float waveOffset = std::sin(waveTime * 1.5f) * 0.1f; // Slight horizontal movement
    
    // Apply wave to the flag (rotate around vertical axis for wind effect)
    transform = glm::rotate(transform, glm::radians(waveAngle), glm::vec3(0, 1, 0));
    transform = glm::translate(transform, glm::vec3(waveOffset, 0.0f, 0.0f));
    
    return transform;
}

void CotCo::raiseFlag()
{
    if (!isRaising && flagHeight < 1.0f)
    {
        isRaising = true;
        isLowering = false;
    }
}

void CotCo::lowerFlag()
{
    if (!isLowering && flagHeight > 0.0f)
    {
        isLowering = true;
        isRaising = false;
    }
}
