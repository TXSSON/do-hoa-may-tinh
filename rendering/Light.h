#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "../rendering/Shader.h"

/**
 * 👤 NGƯỜI 3: Light System
 */

// Ánh sáng theo hướng (mặt trời)
struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirectionalLight();
    void sendToShader(Shader &shader, const std::string &uniformName = "dirLight");
};

// Ánh sáng điểm (đèn đường)
struct PointLight
{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Attenuation
    float constant;
    float linear;
    float quadratic;

    PointLight();
    void sendToShader(Shader &shader, const std::string &uniformName = "pointLight");
};

// Ánh sáng spot (đèn pin, pha...)
struct SpotLight
{
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    SpotLight();
    void sendToShader(Shader &shader, const std::string &uniformName = "spotLight");
};

#endif
