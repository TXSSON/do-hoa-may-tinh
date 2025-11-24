#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Mesh.h"

class Texture;
class Shader;

class Guard
{
public:
    glm::vec3 position;
    float rotationY; // Orientation in degrees
    
    // Body parts
    Mesh* head;
    Mesh* body;
    Mesh* armLeft;
    Mesh* armRight;
    Mesh* legLeft;
    Mesh* legRight;
    Mesh* hat;
    Mesh* rifle;
    
    // Detailed uniform parts
    Mesh* collar;      // Shirt collar
    Mesh* belt;        // Belt around waist
    Mesh* bootLeft;    // Left boot
    Mesh* bootRight;   // Right boot
    Mesh* helmetVisor; // Helmet visor detail

    // Animation state
    float time;

    Guard(glm::vec3 pos, float rotY = 0.0f);
    ~Guard();

    void draw(Shader &shader, Texture* uniformTex, Texture* metalTex, Texture* faceTex);
    void update(float deltaTime);

private:
    void initModel();
};
