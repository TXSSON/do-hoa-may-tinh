#include "StreetLight.h"
#include "Primitives.h"

/**
 * Street light with ornamental bulbs
 */

StreetLight::StreetLight(glm::vec3 pos) : position(pos)
{
    // Metal pole
    float poleHeight = 6.0f;
    float poleRadius = 0.15f;
    pole = Primitives::createCylinder(poleRadius, poleHeight, 12);

    // Light bulbs (spheres at top)
    bulb1 = Primitives::createSphere(0.3f, 16, 8);
    bulb2 = Primitives::createSphere(0.3f, 16, 8);
}

StreetLight::~StreetLight()
{
    delete pole;
    delete bulb1;
    delete bulb2;
}

glm::vec3 StreetLight::getLightPosition()
{
    // Lights are at top of pole
    return position + glm::vec3(0.0f, 6.0f, 0.0f);
}
