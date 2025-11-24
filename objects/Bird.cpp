#include "Bird.h"
#include "Primitives.h"
#include <cmath>

Bird::Bird(glm::vec3 centerPos, float r, float startAngle)
    : center(centerPos), radius(r), flightAngle(startAngle),
      flightSpeed(0.3f), wingFlapTime(0.0f), wingFlapSpeed(4.0f)
{
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
        0.0f,
        radius * std::sin(flightAngle)
    );
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
    // Update flight angle (circular motion)
    flightAngle += flightSpeed * deltaTime;
    
    // Wrap angle
    if (flightAngle > 2.0f * M_PI)
        flightAngle -= 2.0f * M_PI;
    
    // Calculate new position on circle
    position = center + glm::vec3(
        radius * std::cos(flightAngle),
        0.0f,
        radius * std::sin(flightAngle)
    );
    
    // Update wing flapping
    wingFlapTime += wingFlapSpeed * deltaTime;
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

