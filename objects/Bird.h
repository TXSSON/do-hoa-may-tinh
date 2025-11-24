#ifndef BIRD_H
#define BIRD_H

#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Flying bird (pigeon) with wing flapping animation
 * Flies in circular path around a center point
 */
class Bird
{
public:
    Mesh *body;
    Mesh *head;
    Mesh *beak;
    Mesh *tail;
    Mesh *wingLeft;
    Mesh *wingRight;

    Bird(glm::vec3 centerPos, float radius, float startAngle = 0.0f);
    ~Bird();

    void update(float deltaTime);
    void draw(); // Returns transforms for body and wings
    
    glm::vec3 getPosition() const { return position; }
    glm::mat4 getBodyTransform() const;
    glm::mat4 getHeadTransform() const;
    glm::mat4 getBeakTransform() const;
    glm::mat4 getTailTransform() const;
    glm::mat4 getLeftWingTransform() const;
    glm::mat4 getRightWingTransform() const;

private:
    glm::vec3 position;
    glm::vec3 center;        // Center of circular path
    float radius;            // Circle radius
    float flightAngle;       // Current angle in circle (radians)
    float flightSpeed;       // Angular velocity
    float wingFlapTime;      // For wing animation
    float wingFlapSpeed;     // Wing flap frequency
};

#endif
