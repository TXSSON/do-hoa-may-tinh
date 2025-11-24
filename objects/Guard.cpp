#include "Guard.h"
#include "../models/Primitives.h"
#include "../models/Texture.h"
#include "../Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Guard::Guard(glm::vec3 pos, float rotY)
    : position(pos), rotationY(rotY), time(0.0f)
{
    initModel();
}

Guard::~Guard()
{
    delete head;
    delete body;
    delete armLeft;
    delete armRight;
    delete legLeft;
    delete legRight;
    delete hat;
    delete rifle;
    delete collar;
    delete belt;
    delete bootLeft;
    delete bootRight;
    delete helmetVisor;
}

void Guard::initModel()
{
    // Dimensions (approximate for 1.8m tall person)
    // Head: 0.25m size
    head = Primitives::createSphere(0.12f, 20, 20);
    
    // Body: 0.5m width, 0.7m height, 0.25m depth
    body = Primitives::createBox(0.5f, 0.7f, 0.25f);
    
    // Arms: 0.12m width, 0.7m length
    armLeft = Primitives::createBox(0.12f, 0.7f, 0.12f);
    armRight = Primitives::createBox(0.12f, 0.7f, 0.12f);
    
    // Legs: 0.15m width, 0.9m length (pants)
    legLeft = Primitives::createBox(0.18f, 0.9f, 0.18f);
    legRight = Primitives::createBox(0.18f, 0.9f, 0.18f);
    
    // Pith Helmet (Vietnamese ceremonial style)
    hat = Primitives::createSphere(0.15f, 20, 20); // Rounded helmet
    helmetVisor = Primitives::createBox(0.18f, 0.02f, 0.12f); // Visor
    
    // Rifle: Long thin box (AK-47 style)
    rifle = Primitives::createBox(0.05f, 1.2f, 0.1f);
    
    // Uniform details
    collar = Primitives::createBox(0.52f, 0.08f, 0.26f); // Shirt collar
    belt = Primitives::createBox(0.52f, 0.06f, 0.26f);   // Belt
    
    // Boots (polished black)
    bootLeft = Primitives::createBox(0.2f, 0.25f, 0.28f);
    bootRight = Primitives::createBox(0.2f, 0.25f, 0.28f);
}

void Guard::update(float deltaTime)
{
    // Standing at attention - no movement
    // Only subtle breathing animation
    time += deltaTime;
}

void Guard::draw(Shader &shader, Texture* uniformTex, Texture* metalTex, Texture* faceTex)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0, 1, 0));
    
    // Breathing animation (scale Y slightly)
    float breath = 1.0f + sin(time * 2.0f) * 0.005f;
    
    // === BOOTS (Black leather) ===
    if (metalTex) metalTex->bind(0); // Using metalTex for black boots
    
    // Left Boot
    glm::mat4 modelBootL = glm::translate(model, glm::vec3(-0.15f, 0.125f, 0.0f));
    shader.setMat4("model", modelBootL);
    bootLeft->draw();
    
    // Right Boot
    glm::mat4 modelBootR = glm::translate(model, glm::vec3(0.15f, 0.125f, 0.0f));
    shader.setMat4("model", modelBootR);
    bootRight->draw();
    
    // === LEGS (White pants) ===
    if (uniformTex) uniformTex->bind(0);
    
    // Left Leg
    glm::mat4 modelLegL = glm::translate(model, glm::vec3(-0.15f, 0.7f, 0.0f));
    shader.setMat4("model", modelLegL);
    legLeft->draw();
    
    // Right Leg
    glm::mat4 modelLegR = glm::translate(model, glm::vec3(0.15f, 0.7f, 0.0f));
    shader.setMat4("model", modelLegR);
    legRight->draw();
    
    // === BODY (White uniform) ===
    glm::mat4 modelBody = glm::translate(model, glm::vec3(0.0f, 1.15f + 0.35f, 0.0f));
    modelBody = glm::scale(modelBody, glm::vec3(1.0f, breath, 1.0f));
    shader.setMat4("model", modelBody);
    body->draw();
    
    // === COLLAR (White) ===
    glm::mat4 modelCollar = glm::translate(model, glm::vec3(0.0f, 1.54f, 0.0f));
    shader.setMat4("model", modelCollar);
    collar->draw();
    
    // === BELT (Black/Metal) ===
    if (metalTex) metalTex->bind(0);
    glm::mat4 modelBelt = glm::translate(model, glm::vec3(0.0f, 1.15f, 0.0f));
    shader.setMat4("model", modelBelt);
    belt->draw();
    
    // === ARMS (White uniform) - Standing at attention ===
    if (uniformTex) uniformTex->bind(0);
    
    // Left Arm - straight down, holding rifle
    glm::mat4 modelArmL = glm::translate(model, glm::vec3(-0.32f, 1.15f, 0.0f));
    shader.setMat4("model", modelArmL);
    armLeft->draw();
    
    // Right Arm - straight down at side
    glm::mat4 modelArmR = glm::translate(model, glm::vec3(0.32f, 1.15f, 0.0f));
    shader.setMat4("model", modelArmR);
    armRight->draw();

    // === HEAD (Skin tone) ===
    if (faceTex) faceTex->bind(0);
    glm::mat4 modelHead = glm::translate(model, glm::vec3(0.0f, 1.68f + (breath - 1.0f), 0.0f));
    shader.setMat4("model", modelHead);
    head->draw();
    
    // === HELMET (Golden yellow pith helmet) ===
    if (metalTex) metalTex->bind(0); // Will use golden texture
    glm::mat4 modelHat = glm::translate(model, glm::vec3(0.0f, 1.85f + (breath - 1.0f), 0.0f));
    shader.setMat4("model", modelHat);
    hat->draw();
    
    // Helmet Visor
    glm::mat4 modelVisor = glm::translate(model, glm::vec3(0.0f, 1.78f + (breath - 1.0f), 0.14f));
    shader.setMat4("model", modelVisor);
    helmetVisor->draw();
    
    // === RIFLE (Metal) - Held at side ===
    glm::mat4 modelRifle = glm::translate(model, glm::vec3(-0.38f, 0.9f, 0.0f));
    modelRifle = glm::rotate(modelRifle, glm::radians(5.0f), glm::vec3(0, 0, 1)); // Slight angle
    shader.setMat4("model", modelRifle);
    rifle->draw();
}

