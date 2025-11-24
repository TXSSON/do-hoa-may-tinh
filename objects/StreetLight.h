#ifndef STREETLIGHT_H
#define STREETLIGHT_H

#include "Mesh.h"
#include <glm/glm.hpp>

/**
 * Street light - pole with light bulbs
 */
class StreetLight
{
public:
    Mesh *pole;
    Mesh *bulb1;
    Mesh *bulb2; // Multiple bulbs like in reference
    glm::vec3 position;

    StreetLight(glm::vec3 pos);
    ~StreetLight();

    glm::vec3 getLightPosition(); // For point light
};

#endif
