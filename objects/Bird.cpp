#include "Bird.h"
#include "Primitives.h"
#include <cmath>

Bird::Bird(glm::vec3 centerPos, float r, float startAngle)
    : center(centerPos), radius(r), flightAngle(startAngle),
      flightSpeed(0.3f), wingFlapTime(0.0f), wingFlapSpeed(8.0f), // Faster wings
      state(FLYING), baseHeight(centerPos.y), height(centerPos.y),
      groundTime(0.0f), flightTime(0.0f)
{
    // Randomize landing time (10-30 seconds)
    nextLandingTime = 10.0f + (rand() % 200) / 10.0f;
    maxGroundTime = 5.0f + (rand() % 50) / 10.0f; // 5-10 seconds on ground

    // Body - plump pigeon body (pigeons are rounder)
    body = Primitives::createSphere(0.3f, 20, 20);
    
    // Head - small pigeon head
    head = Primitives::createSphere(0.12f, 12, 12);
    
    // Beak - short pigeon beak
    beak = Primitives::createBox(0.04f, 0.04f, 0.12f);
    
    // Tail - pigeon tail (shorter and more compact)
    tail = Primitives::createBox(0.25f, 0.05f, 0.18f);
    
    // Wings - pigeon wings (proportional to body)
    wingLeft = Primitives::createBox(0.5f, 0.05f, 0.35f);
    wingRight = Primitives::createBox(0.5f, 0.05f, 0.35f);
    
    // Initialize position
    position = center + glm::vec3(
        radius * std::cos(flightAngle),
        0.0f, // Relative to center Y
        radius * std::sin(flightAngle)
    );
    position.y = height;
}

Bird::~Bird()
{
    delete body;
    delete head;
    delete beak;
    delete tail;
    delete wingLeft;
    delete wingRight;
}

void Bird::update(float deltaTime)
{
    // Update flight angle (circular motion) - always move forward unless on ground
    if (state != ON_GROUND) {
        flightAngle += flightSpeed * deltaTime;
        
        // Wrap angle
        if (flightAngle > 2.0f * M_PI)
            flightAngle -= 2.0f * M_PI;
            
        // Update wing flapping
        wingFlapTime += wingFlapSpeed * deltaTime;
    } else {
        // On ground: occasional small movements or peck?
        // For now just stop flapping
        wingFlapTime = 0; 
    }
    
    // State Machine
    switch (state) {
        case FLYING:
            flightTime += deltaTime;
            // Return to cruising altitude if needed
            if (height < baseHeight) {
                height += 2.0f * deltaTime;
                if (height > baseHeight) height = baseHeight;
            }
            
            // Check if it's time to land
            if (flightTime > nextLandingTime) {
                state = LANDING;
                flightTime = 0;
            }
            break;
            
        case LANDING:
            // Descend
            height -= 3.0f * deltaTime; // Descent speed
            if (height <= 0.5f) { // Ground level (approx)
                height = 0.5f;
                state = ON_GROUND;
                groundTime = 0;
            }
            break;
            
        case ON_GROUND:
            groundTime += deltaTime;
            if (groundTime > maxGroundTime) {
                state = TAKING_OFF;
                // Reset next landing time
                nextLandingTime = 15.0f + (rand() % 200) / 10.0f;
            }
            break;
            
        case TAKING_OFF:
            // Ascend
            height += 4.0f * deltaTime; // Ascent speed
            if (height >= baseHeight) {
                height = baseHeight;
                state = FLYING;
            }
            break;
    }
    
    // Calculate new position
    // X and Z based on circle, Y based on height
    float currentRadius = radius;
    // When landing/on ground, maybe spiral in slightly? keeping simple for now
    
    position.x = center.x + currentRadius * std::cos(flightAngle);
    position.z = center.z + currentRadius * std::sin(flightAngle);
    position.y = height;
}

glm::mat4 Bird::getBodyTransform() const
{
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);
    
    // Face direction of movement
    float directionAngle = flightAngle + M_PI / 2.0f;
    transform = glm::rotate(transform, directionAngle, glm::vec3(0, 1, 0));
    
    return transform;
}

glm::mat4 Bird::getLeftWingTransform() const
{
    // Wing flap: sine wave between -30° and +30°
    float flapAngle = std::sin(wingFlapTime) * 30.0f;
    
    glm::mat4 transform = getBodyTransform();
    
    // Position wing on left side of body
    transform = glm::translate(transform, glm::vec3(-0.3f, 0.0f, 0.0f));
    
    // Rotate wing (flapping motion)
    transform = glm::rotate(transform, glm::radians(flapAngle), glm::vec3(0, 0, 1));
    
    // Orient wing horizontally
    transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(1, 0, 0));
    
    return transform;
}

glm::mat4 Bird::getRightWingTransform() const
{
    // Symmetric to left wing
    float flapAngle = std::sin(wingFlapTime) * 30.0f;
    
    glm::mat4 transform = getBodyTransform();
    
    // Position wing on right side
    transform = glm::translate(transform, glm::vec3(0.3f, 0.0f, 0.0f));
    
    // Rotate wing (opposite direction for symmetry)
    transform = glm::rotate(transform, glm::radians(-flapAngle), glm::vec3(0, 0, 1));
    
    // Orient wing horizontally
    transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(1, 0, 0));
    
    return transform;
}

glm::mat4 Bird::getHeadTransform() const
{
    glm::mat4 transform = getBodyTransform();
    
    // Position head in front of body (adjusted for larger body)
    transform = glm::translate(transform, glm::vec3(0.0f, 0.08f, 0.4f));
    
    return transform;
}

glm::mat4 Bird::getBeakTransform() const
{
    glm::mat4 transform = getBodyTransform();
    
    // Position beak at front of head (adjusted for pigeon)
    transform = glm::translate(transform, glm::vec3(0.0f, 0.08f, 0.54f));
    
    return transform;
}

glm::mat4 Bird::getTailTransform() const
{
    glm::mat4 transform = getBodyTransform();
    
    // Position tail at back of body (adjusted for larger body)
    transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -0.4f));
    
    // Angle tail slightly upward
    transform = glm::rotate(transform, glm::radians(-10.0f), glm::vec3(1, 0, 0));
    
    return transform;
}

void Bird::draw()
{
    // This method is empty - transforms are retrieved individually
    // in main.cpp for rendering
}

