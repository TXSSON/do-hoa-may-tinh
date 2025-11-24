// main.cpp - Lang Bac Scene V5.0 - Visual Polish & Guards

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Primitives.h"
#include "objects/Lang.h"
#include "objects/CotCo.h"
#include "objects/StreetLight.h"
#include "TimeOfDay.h"
#include "objects/Cloud.h"
#include "objects/Bird.h"
#include "objects/Tree.h"
#include "objects/Fence.h"
#include "objects/Guard.h"

#include <iostream>
#include <vector>
#include <ctime>

// Function prototypes
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// Camera
Camera camera(glm::vec3(0.0f, 30.0f, 70.0f)); // Centered in yard, elevated for better view
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Keyboard state
float lastTKeyPress = 0.0f;
float lastUKeyPress = 0.0f;
float lastLKeyPress = 0.0f;

// Scene Objects (Global for RenderScene access)
Mesh *pavement = nullptr;
Mesh *sharedGrassPatch = nullptr;
std::vector<glm::mat4> grassTransforms;
Mesh *pathway = nullptr;
// grassGridLines removed
LangBac *langBac = nullptr;
Mesh *skyDome = nullptr;
Shader *skyShader = nullptr;
Shader *cloudShader = nullptr;
CotCo *cotCo = nullptr;
std::vector<StreetLight *> lights;
std::vector<Guard *> guards;
std::vector<Cloud *> clouds;
std::vector<Bird *> birds;
std::vector<Tree *> trees;
std::vector<Mesh *> redFlags;
std::vector<Fence *> fences;
std::vector<Mesh *> backgroundBuildings;
std::vector<glm::mat4> buildingTransforms;
std::vector<Mesh *> textBanners;

// Textures (Global)
Texture *grassTexture = nullptr;
Texture *stoneTexture = nullptr;
Texture *concreteTexture = nullptr;
Texture *flagTexture = nullptr;
Texture *metalTexture = nullptr;
Texture* yellowTexture = nullptr; // Inner walls
Texture* textTexture = nullptr; // Text on roof
Texture* doorTexture = nullptr; // Entrance door
Texture *cloudTexture = nullptr;
Texture *birdTexture = nullptr;
Texture *guardUniformTexture = nullptr;
Texture *guardHelmetTexture = nullptr;
Texture *guardBootsTexture = nullptr;
Texture *redCarpetTexture = nullptr; // Solid red texture for carpet
// ===== PERLIN NOISE IMPLEMENTATION =====
// Simple implementation for procedural cloud generation
float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
float lerp(float t, float a, float b) { return a + t * (b - a); }
float grad(int hash, float x, float y, float z) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// Permutation table
int p[512];
void initNoise() {
    int permutation[] = { 151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };
    for (int i=0; i < 256 ; i++) p[256+i] = p[i] = permutation[i];
}

float noise(float x, float y, float z) {
    int X = (int)floor(x) & 255, Y = (int)floor(y) & 255, Z = (int)floor(z) & 255;
    x -= floor(x); y -= floor(y); z -= floor(z);
    float u = fade(x), v = fade(y), w = fade(z);
    int A = p[X]+Y, AA = p[A]+Z, AB = p[A+1]+Z, B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;
    return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),  grad(p[BA  ], x-1, y  , z   )),
                           lerp(u, grad(p[AB  ], x  , y-1, z   ),  grad(p[BB  ], x-1, y-1, z   ))),
                   lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  grad(p[BA+1], x-1, y  , z-1 )),
                           lerp(u, grad(p[AB+1], x  , y-1, z-1 ),  grad(p[BB+1], x-1, y-1, z-1 ))));
}

// Fractal Brownian Motion
float fbm(float x, float y, float z, int octaves) {
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;
    for(int i=0;i<octaves;i++) {
        total += noise(x * frequency, y * frequency, z * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= 0.5f;
        frequency *= 2.0f;
    }
    return total/maxValue;
}

Texture *treeBarkTexture = nullptr;
Texture *treeLeavesTexture = nullptr;

// Function to render the scene (used for both Shadow Pass and Lighting Pass)
void RenderScene(Shader &shader, bool isNight = false)
{
    // ===== RENDER SKY DOME (Only in Lighting Pass, not Shadow Pass) =====
    // We detect if it's lighting pass by checking if shader is NOT the depth shader
    // But RenderScene takes generic shader.
    // We should move Sky Dome rendering OUT of RenderScene or add a flag.
    // For simplicity, we'll assume if the shader program ID matches the main shader, we draw sky.
    // Actually, RenderScene is called with `shader` (main) and `depthShader`.
    // We can just check a uniform or pass a bool.
    // Or better: Render Sky Dome in the main loop BEFORE calling RenderScene.
    // BUT RenderScene is convenient.
    // Set default object color (white)
    shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

    // Let's render Sky Dome in main loop, NOT here.
    
    // ===== RENDER GROUND & PAVEMENT =====
    // 1. Pavement (Concrete texture) - Base layer
    if (concreteTexture) concreteTexture->bind(0);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f)); // Slightly below 0
    shader.setMat4("model", model);
    if (pavement) pavement->draw();
    
    // 2. Grass Patches (Grass texture)    // Grass patches rendering - DISABLED per user request
    // 2. Grass Patches (Grass texture)
    if (grassTexture) grassTexture->bind(0);
    shader.setMat4("model", glm::mat4(1.0f));
    
    if (sharedGrassPatch)
    {
        for (const auto& transform : grassTransforms)
        {
            shader.setMat4("model", transform);
            sharedGrassPatch->draw();
        }
    }
    
    // 3. Central Pathway (Stone texture) - DISABLED to avoid overlap with horizontal walkway
    /*
    if (stoneTexture) stoneTexture->bind(0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.06f, 0.0f)); // Slightly above grass
    shader.setMat4("model", model);
    if (pathway) pathway->draw();
    */

    // ===== RENDER LANG BAC (PHOTO-ACCURATE) =====
    if (stoneTexture) stoneTexture->bind(0);
    
    // Ground platform - mesh 0
    glm::mat4 modelPlatform = glm::mat4(1.0f);
    modelPlatform = glm::translate(modelPlatform, glm::vec3(0.0f, 0.25f, -10.0f));
    shader.setMat4("model", modelPlatform);
    if (langBac) langBac->meshes[0]->draw();
    
    // Stairs (5 steps) - meshes 1-5
    // Use stone texture for stairs
    if (stoneTexture) stoneTexture->bind(0);
    
    int numSteps = 5;
    for (int i = 0; i < numSteps; i++)
    {
        glm::mat4 modelStep = glm::mat4(1.0f);
        float yPos = 0.65f + i * 0.3f;  // Comfortable rise (0.65, 0.95, 1.25, 1.55, 1.85)
        // Fix Z-order: Steps should go UP towards the mausoleum (Z=-10)
        // Lowest step (i=0) should be far (Z=13), Highest step (i=4) should be close (Z=1)
        float zPos = 13.0f - i * 3.0f;   
        modelStep = glm::translate(modelStep, glm::vec3(0.0f, yPos, zPos));
        shader.setMat4("model", modelStep);
        if (langBac) langBac->meshes[1 + i]->draw();
    }
    
    // Red carpet runner on stairs
    if (redCarpetTexture) redCarpetTexture->bind(0);
    for (int i = 0; i < numSteps; i++)
    {
        glm::mat4 modelCarpet = glm::mat4(1.0f);
        float yPos = 0.65f + i * 0.3f + 0.16f; // Slightly above step
        float zPos = 13.0f - i * 3.0f; // Match steps
        modelCarpet = glm::translate(modelCarpet, glm::vec3(0.0f, yPos, zPos));
        shader.setMat4("model", modelCarpet);
        
        // Draw thin red carpet (8 units wide, 0.05 tall, 3.0 deep)
        Mesh* carpet = Primitives::createBox(8.0f, 0.05f, 3.0f);
        carpet->draw();
        delete carpet;
    }
    
    // Lower Tier (base with entrance) - mesh 6
    // Use STONE texture for gray granite appearance
    if (stoneTexture) stoneTexture->bind(0);
    glm::mat4 modelLowerTier = glm::mat4(1.0f);
    modelLowerTier = glm::translate(modelLowerTier, glm::vec3(0.0f, 3.5f, -10.0f));
    shader.setMat4("model", modelLowerTier);
    if (langBac) langBac->meshes[6]->draw();
    
    // ===== RENDER CONCRETE WALKWAY =====
    // ===== RENDER CONCRETE WALKWAY =====
    // Horizontal concrete walkway in front of mausoleum
    if (concreteTexture) concreteTexture->bind(0);
    
    // Darker concrete for walkway (distinguish from ground)
    shader.setVec3("objectColor", glm::vec3(0.6f, 0.6f, 0.6f));
    
    glm::mat4 modelWalkway = glm::mat4(1.0f);
    // Position: Horizontal across front (X=-100 to X=100), at Z=25
    // Center at X=0, Z=25, Width 200 units (along X), Depth 20 units (along Z)
    // Reduced width from 300 to 200 to match ground width
    modelWalkway = glm::translate(modelWalkway, glm::vec3(0.0f, 0.02f, 25.0f)); 
    shader.setMat4("model", modelWalkway);
    // Use separate tiling for X and Z to keep texture square and smooth
    // Width 200 -> Tiling 20 (10 units/repeat)
    // Depth 20 -> Tiling 2 (10 units/repeat)
    Mesh* concreteWalkway = Primitives::createPlane(200.0f, 20.0f, 20.0f, 2.0f); 
    concreteWalkway->draw();
    delete concreteWalkway;
    
    // Reset object color to white
    shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

    // ===== RENDER GRASS =====
    // Grass rendering disabled per user request
    /*
    if (grassTexture) grassTexture->bind(0);
    glm::mat4 modelGrass = glm::mat4(1.0f);
    modelGrass = glm::translate(modelGrass, glm::vec3(0.0f, 0.0f, 5.0f)); 
    shader.setMat4("model", modelGrass);
    Mesh* grassPlane = Primitives::createPlane(200.0f, 20.0f, 10.0f);
    grassPlane->draw();
    delete grassPlane;
    */
    
    // ===== RENDER RESIDENTIAL HOUSES =====
    // Houses removed per user request
    /*
    // Left end houses (X < -100)
    for (int i = 0; i < 8; i++) {
        float x = -110.0f - i * 15.0f; // X positions: -110, -125, -140, ...
        float z = 25.0f; // Along walkway
        
        // House base (walls)
        if (stoneTexture) stoneTexture->bind(0);
        glm::mat4 modelHouse = glm::mat4(1.0f);
        modelHouse = glm::translate(modelHouse, glm::vec3(x, 3.0f, z));
        shader.setMat4("model", modelHouse);
        Mesh* houseBase = Primitives::createBox(8.0f, 6.0f, 10.0f);
        houseBase->draw();
        delete houseBase;
        
        // Roof
        if (metalTexture) metalTexture->bind(0);
        glm::mat4 modelRoof = glm::mat4(1.0f);
        modelRoof = glm::translate(modelRoof, glm::vec3(x, 7.0f, z));
        shader.setMat4("model", modelRoof);
        Mesh* roof = Primitives::createBox(9.0f, 2.0f, 11.0f);
        roof->draw();
        delete roof;
    }
    
    // Right end houses (X > 100)
    for (int i = 0; i < 8; i++) {
        float x = 110.0f + i * 15.0f; // X positions: 110, 125, 140, ...
        float z = 25.0f;
        
        // House base
        if (stoneTexture) stoneTexture->bind(0);
        glm::mat4 modelHouse = glm::mat4(1.0f);
        modelHouse = glm::translate(modelHouse, glm::vec3(x, 3.0f, z));
        shader.setMat4("model", modelHouse);
        Mesh* houseBase = Primitives::createBox(8.0f, 6.0f, 10.0f);
        houseBase->draw();
        delete houseBase;
        
        // Roof
        if (metalTexture) metalTexture->bind(0);
        glm::mat4 modelRoof = glm::mat4(1.0f);
        modelRoof = glm::translate(modelRoof, glm::vec3(x, 7.0f, z));
        shader.setMat4("model", modelRoof);
        Mesh* roof = Primitives::createBox(9.0f, 2.0f, 11.0f);
        roof->draw();
        delete roof;
    }
    */
    
    // ===== RENDER GRANDSTANDS (Khán đài) =====
    // Grandstands on left and right sides, aligned with mausoleum
    // Use concrete texture for grandstands
    if (concreteTexture) concreteTexture->bind(0);
    
    // Left Grandstand (left side of mausoleum)
    // Aligned with mausoleum, not overlapping walkway
    for(int i=0; i<5; i++) { // 5 tiers (reduced from 7)
        glm::mat4 modelStand = glm::mat4(1.0f);
        // Position: Left of mausoleum
        float yHeight = 2.0f + i * 1.5f; // Height increases (start at 2.0 instead of 0.5)
        float xPos = -50.0f; // Fixed X position (left side)
        float zPos = 5.0f - i * 3.0f; // Moved back (tier 0 at Z=5, tier 4 at Z=-7)
        // Extended grandstand along X-axis
        modelStand = glm::translate(modelStand, glm::vec3(xPos, yHeight, zPos));
        shader.setMat4("model", modelStand);
        Mesh* standTier = Primitives::createBox(60.0f, 2.0f, 10.0f); // Long along X (60 units), depth 10
        standTier->draw();
        delete standTier;
    }

    // Right Grandstand (right side of mausoleum)
    // Aligned with mausoleum, not overlapping walkway
    for(int i=0; i<5; i++) { // 5 tiers (reduced from 7)
        glm::mat4 modelStand = glm::mat4(1.0f);
        // Position: Right of mausoleum
        float yHeight = 2.0f + i * 1.5f; // Height increases (start at 2.0 instead of 0.5)
        float xPos = 50.0f; // Fixed X position (right side)
        float zPos = 5.0f - i * 3.0f; // Moved back (tier 0 at Z=5, tier 4 at Z=-7)
        // Extended grandstand along X-axis
        modelStand = glm::translate(modelStand, glm::vec3(xPos, yHeight, zPos));
        shader.setMat4("model", modelStand);
        Mesh* standTier = Primitives::createBox(60.0f, 2.0f, 10.0f); // Long along X (60 units), depth 10
        standTier->draw();
        delete standTier;
    }

    // Entrance recess - mesh 7 (darker interior)
    if (metalTexture) metalTexture->bind(0);
    glm::mat4 modelRecess = glm::mat4(1.0f);
    modelRecess = glm::translate(modelRecess, glm::vec3(0.0f, 4.0f, -2.25f));
    shader.setMat4("model", modelRecess);
    if (langBac) langBac->meshes[7]->draw();
    
    // Door removed as requested
    
    // UPPER STAIRS (Tier 1 to Tier 2) - meshes 8-10
    // Use stone texture
    if (stoneTexture) stoneTexture->bind(0);
    for (int i = 0; i < 3; i++)
    {
        glm::mat4 modelUpStep = glm::mat4(1.0f);
        // Position on top of lower tier (Y=6.0)
        float yPos = 6.25f + i * 0.5f; 
        modelUpStep = glm::translate(modelUpStep, glm::vec3(0.0f, yPos, -10.0f));
        shader.setMat4("model", modelUpStep);
        if (langBac) langBac->meshes[8 + i]->draw();
    }
    
    // Upper Tier (Inner Yellow Walls) - mesh 11
    // Use YELLOW texture
    if (yellowTexture) yellowTexture->bind(0);
    glm::mat4 modelUpperTier = glm::mat4(1.0f);
    // Centered inside columns (Z center approx -10.0)
    modelUpperTier = glm::translate(modelUpperTier, glm::vec3(0.0f, 10.5f, -10.0f));
    shader.setMat4("model", modelUpperTier);
    if (langBac) langBac->meshes[11]->draw();
    
    // 20 SQUARE COLUMNS - meshes 12-31
    // Use metal texture for bronze/brown appearance
    if (metalTexture) metalTexture->bind(0);
    
    int meshIndex = 12;
    
    // 4 CORNER COLUMNS
    float corners[4][2] = {
        {-10.0f, -3.0f},   // Front-left
        {10.0f, -3.0f},    // Front-right
        {-10.0f, -17.0f},  // Back-left
        {10.0f, -17.0f}    // Back-right
    };
    
    for (int i = 0; i < 4; i++)
    {
        glm::mat4 modelCol = glm::mat4(1.0f);
        modelCol = glm::translate(modelCol, glm::vec3(corners[i][0], 10.5f, corners[i][1]));
        shader.setMat4("model", modelCol);
        
        if (langBac) langBac->meshes[meshIndex++]->draw();
        
    }
    
    
    // FRONT SIDE - 4 columns (between corners)
    for (int i = 0; i < 4; i++)
    {
        float x = -6.0f + i * 4.0f; // Evenly spaced
        glm::mat4 modelCol = glm::mat4(1.0f);
        modelCol = glm::translate(modelCol, glm::vec3(x, 10.5f, -3.0f));
        shader.setMat4("model", modelCol);
        if (langBac) langBac->meshes[meshIndex++]->draw();
    }
    
    // BACK SIDE - 4 columns
    for (int i = 0; i < 4; i++)
    {
        float x = -6.0f + i * 4.0f;
        glm::mat4 modelCol = glm::mat4(1.0f);
        modelCol = glm::translate(modelCol, glm::vec3(x, 10.5f, -17.0f));
        shader.setMat4("model", modelCol);
        if (langBac) langBac->meshes[meshIndex++]->draw();
    }
    
    // LEFT SIDE - 4 columns
    for (int i = 0; i < 4; i++)
    {
        float z = -6.0f - i * 3.5f; // Evenly spaced
        glm::mat4 modelCol = glm::mat4(1.0f);
        modelCol = glm::translate(modelCol, glm::vec3(-10.0f, 10.5f, z));
        shader.setMat4("model", modelCol);
        if (langBac) langBac->meshes[meshIndex++]->draw();
    }
    
    // RIGHT SIDE - 4 columns
    for (int i = 0; i < 4; i++)
    {
        float z = -6.0f - i * 3.5f;
        glm::mat4 modelCol = glm::mat4(1.0f);
        modelCol = glm::translate(modelCol, glm::vec3(10.0f, 10.5f, z));
        shader.setMat4("model", modelCol);
        if (langBac) langBac->meshes[meshIndex++]->draw();
    }
    
    // Roof - meshes 32-33
    if (stoneTexture) stoneTexture->bind(0);
    // Roof base
    glm::mat4 modelRoofBase = glm::mat4(1.0f);
    modelRoofBase = glm::translate(modelRoofBase, glm::vec3(0.0f, 15.0f, -10.0f));
    shader.setMat4("model", modelRoofBase);
    if (langBac) langBac->meshes[32]->draw();
    
    // Roof overhang (Facade)
    glm::mat4 modelRoofOverhang = glm::mat4(1.0f);
    // Center at 15.75 (Bottom 14.5, Top 17.0) - sits right on top of columns
    modelRoofOverhang = glm::translate(modelRoofOverhang, glm::vec3(0.0f, 15.75f, -10.0f));
    shader.setMat4("model", modelRoofOverhang);
    if (langBac) langBac->meshes[33]->draw();
    
    // Text rendering removed - clean roof facade as in reality

    // ===== RENDER COT CO =====
    // Base
    if (stoneTexture) stoneTexture->bind(0);
    glm::mat4 modelFlagBase = glm::mat4(1.0f);
    modelFlagBase = glm::translate(modelFlagBase, cotCo->position);
    shader.setMat4("model", modelFlagBase);
    if (cotCo) cotCo->base->draw();

    // Pole
    if (metalTexture) metalTexture->bind(0);
    glm::mat4 modelPole = glm::mat4(1.0f);
    modelPole = glm::translate(modelPole, cotCo->position + glm::vec3(0.0f, 12.75f, 0.0f));
    shader.setMat4("model", modelPole);
    if (cotCo) cotCo->pole->draw();

    // Flag at top
    if (flagTexture) flagTexture->bind(0);
    shader.setMat4("model", cotCo->getFlagTransform()); 
    if (cotCo) cotCo->flag->draw();

    // ===== RENDER STREET LIGHTS =====
    // Street lights re-enabled
    for (auto light : lights)
    {
        // Pole
        if (metalTexture) metalTexture->bind(0);
        glm::mat4 modelLightPole = glm::mat4(1.0f);
        modelLightPole = glm::translate(modelLightPole, light->position + glm::vec3(0.0f, 3.0f, 0.0f));
        shader.setMat4("model", modelLightPole);
        light->pole->draw();

        // Bulbs - Enable emissive glow at night
        shader.setBool("enableBulbGlow", isNight);
        shader.setVec3("objectColor", glm::vec3(1.0f, 0.9f, 0.5f)); // Warm yellow
        
        glm::mat4 modelBulb1 = glm::mat4(1.0f);
        modelBulb1 = glm::translate(modelBulb1, light->getLightPosition() + glm::vec3(-0.5f, 0.0f, 0.0f));
        shader.setMat4("model", modelBulb1);
        light->bulb1->draw();

        glm::mat4 modelBulb2 = glm::mat4(1.0f);
        modelBulb2 = glm::translate(modelBulb2, light->getLightPosition() + glm::vec3(0.5f, 0.0f, 0.0f));
        shader.setMat4("model", modelBulb2);
        light->bulb2->draw();
        
        // Disable bulb glow and reset color
        shader.setBool("enableBulbGlow", false);
        shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
    }

    // ===== RENDER BIRDS =====
    if (birdTexture) birdTexture->bind(0);
    for (auto bird : birds)
    {
        // Body
        shader.setMat4("model", bird->getBodyTransform());
        bird->body->draw();
        
        // Head
        shader.setMat4("model", bird->getHeadTransform());
        bird->head->draw();
        
        // Beak
        shader.setMat4("model", bird->getBeakTransform());
        bird->beak->draw();
        
        // Tail
        shader.setMat4("model", bird->getTailTransform());
        bird->tail->draw();
        
        // Left Wing
        shader.setMat4("model", bird->getLeftWingTransform());
        bird->wingLeft->draw();
        
        // Right Wing
        shader.setMat4("model", bird->getRightWingTransform());
        bird->wingRight->draw();
    }

    // ===== RENDER TREES =====
    // Render ALL trees (removed limit)
    for (auto tree : trees)
    {
        tree->draw(shader, treeBarkTexture, treeLeavesTexture);
    }
    
    // Small flags removed as requested for Scene Layout Redesign
    
    // ===== RENDER FENCES (Decorative) =====
    // Fences along yard perimeter
    if (metalTexture) metalTexture->bind(0);
    
    // Set fence color to Green as requested
    shader.setVec3("objectColor", glm::vec3(0.0f, 0.5f, 0.0f));
    
    for (auto fence : fences)
    {
        fence->draw(shader);
    }
    
    // Reset object color to white
    shader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
    
    // ===== RENDER BACKGROUND BUILDINGS =====
    // Use metal texture for modern look (glass/steel)
    if (metalTexture) metalTexture->bind(0);
    
    // Enable window lights ONLY for buildings
    shader.setBool("enableWindowLights", true);
    
    for (size_t i = 0; i < backgroundBuildings.size(); i++)
    {
        if (i < buildingTransforms.size())
        {
            shader.setMat4("model", buildingTransforms[i]);
            backgroundBuildings[i]->draw();
        }
    }
    
    // Disable window lights for other objects
    shader.setBool("enableWindowLights", false);
    
    // ===== RENDER TEXT BANNERS =====
    // Text banners DISABLED per user request (flags at grandstands)
    /*
    if (flagTexture) flagTexture->bind(0); // Use red/yellow flag texture for banners
    // Banner 1 (Left)
    if (textBanners.size() > 0)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-30.0f, 5.0f, 10.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        textBanners[0]->draw();
    }
    // Banner 2 (Right)
    if (textBanners.size() > 1)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(30.0f, 5.0f, 10.0f));
        model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        textBanners[1]->draw();
    }
    */


    // ===== RENDER GUARDS =====
    for (auto guard : guards)
    {
        guard->draw(shader, guardUniformTexture, guardHelmetTexture, stoneTexture);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()

{
    // =====GLFW Init=====
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Open window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Do An Do Hoa May Tinh - Lang Bac", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // Capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configure global opengl state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // CRITICAL: Scope block
    {


    // Build and compile shaders
    Shader lightingShader("../shaders/lighting_v4.vs", "../shaders/lighting_v4.fs"); // Use v4 lighting
    Shader shadowShader("../shaders/shadow_mapping_depth.vs", "../shaders/shadow_mapping_depth.fs");
    Shader* cloudShader = new Shader("../shaders/cloud.vs", "../shaders/cloud.fs"); // New cloud shader
    Shader* skyShader = new Shader("../shaders/sky.vs", "../shaders/sky.fs"); // Sky shader


    // Load textures
    stoneTexture = new Texture("../assets/textures/stone.png");
    grassTexture = new Texture("../assets/textures/grass.png");
    metalTexture = new Texture("../assets/textures/metal.png");
    flagTexture = new Texture("../assets/textures/vietnam_flag.png");
    
    // Load concrete texture
    concreteTexture = new Texture("../assets/textures/concrete.png");
    birdTexture = new Texture("../assets/textures/pigeon.png");
    treeBarkTexture = new Texture("../assets/textures/tree_bark.png");
    treeLeavesTexture = new Texture("../assets/textures/tree_leaves.png");
    
    // Guard textures
    guardUniformTexture = new Texture("../assets/textures/guard_uniform.png");
    guardHelmetTexture = new Texture("../assets/textures/guard_helmet.png");
    // Reuse metal for boots/gun or load specific if needed (using metal for now as placeholder if not loaded)
    // Actually we have guard_boots.png
    Texture *guardBootsTexture = new Texture("../assets/textures/guard_boots.png");
    
    // Additional textures
    // Additional textures
    unsigned char* redData = new unsigned char[3]{160, 0, 0};
    redCarpetTexture = new Texture(1, 1, redData, GL_RGB);
    delete[] redData;
    
    unsigned char* yellowData = new unsigned char[3]{255, 200, 0};
    yellowTexture = new Texture(1, 1, yellowData, GL_RGB);
    delete[] yellowData;
    
    unsigned char* whiteData = new unsigned char[3]{255, 255, 255};
    textTexture = new Texture(1, 1, whiteData, GL_RGB);
    delete[] whiteData;
    
    unsigned char* doorData = new unsigned char[3]{40, 30, 20};
    doorTexture = new Texture(1, 1, doorData, GL_RGB);
    delete[] doorData;

    // Set texture filtering for pixel art style or sharp edges
    birdTexture->setFiltering(GL_NEAREST, GL_NEAREST);
    
    // Create objects
    langBac = new LangBac();
    cotCo = new CotCo(glm::vec3(0.0f, 0.0f, 30.0f));

    pavement = Primitives::createPlane(200.0f, 200.0f);
    
    // Create shared grass patch for better performance
    sharedGrassPatch = Primitives::createPlane(2.0f, 2.0f); // Small patch
    
    // Generate grass positions (simple grid with some randomness)
    // REMOVED - Legacy random grass generation caused overlap with new grid layout
    /*
    for (int i = 0; i < 500; i++)
    {
        float x = -100.0f + (rand() % 2000) / 10.0f;
        float z = -100.0f + (rand() % 2000) / 10.0f;
        
        // Don't place grass on the pavement/road/building area
        // Building area: X[-30, 30], Z[-30, 10]
        // Road area: Z[15, 55]
        if ((x > -30 && x < 30 && z > -30 && z < 10) || (z > 15 && z < 55)) continue;
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, 0.01f, z));
        model = glm::rotate(model, glm::radians((float)(rand() % 360)), glm::vec3(0.0f, 1.0f, 0.0f));
        grassTransforms.push_back(model);
    }
    */
    
    // Create Street Lights - Legacy loop removed to prevent duplicates
    // Lights are now handled in the grass area section (around line 900)
    
    // Create Birds
    for (int i = 0; i < 15; i++)
    {
        birds.push_back(new Bird(glm::vec3(0.0f, 20.0f + (rand()%10), -10.0f + (rand()%20)), 30.0f + (rand()%10), i * 0.5f));
    }

    

    
    // Trees are now created later in the initialization (lines 881-904)
    // All trees positioned behind mausoleum and grandstands (Z < -15)
    
    
    // Create Fences (Decorative)
    // Fences removed/commented out in render loop
    /*
    fences.push_back(new Fence(glm::vec3(-40.0f, 0.0f, 40.0f), glm::vec3(40.0f, 0.0f, 40.0f), 1.5f));
    // ...
    */
            
        // Initialize Noise
        initNoise();
        
        // Generate Procedural Cloud Texture using FBM Noise
        const int cloudTexSize = 1024; // Higher resolution for sharper details
        unsigned char* cloudData = new unsigned char[cloudTexSize * cloudTexSize * 3]; // RGB
        
        for (int y = 0; y < cloudTexSize; y++) {
            for (int x = 0; x < cloudTexSize; x++) {
                // Normalized coordinates
                float u = (float)x / cloudTexSize;
                float v = (float)y / cloudTexSize;
                
                // Distance from center for radial mask
                float cx = u * 2.0f - 1.0f;
                float cy = v * 2.0f - 1.0f;
                float dist = sqrt(cx*cx + cy*cy);
                
                // FBM Noise with more octaves for detail
                float scale = 6.0f; 
                float n = fbm(u * scale, v * scale, 0.0f, 7); // 7 octaves for more detail
                
                // Map noise from [-1, 1] to [0, 1]
                n = n * 0.5f + 0.5f;
                
                float intensity = n;
                
                // Apply radial mask to make edges soft and transparent
                float mask = 1.0f - glm::smoothstep(0.3f, 0.95f, dist);
                intensity = n * mask;
                
                // Smooth thresholding for softer clouds
                intensity = glm::smoothstep(0.15f, 0.65f, intensity);
                
                // Boost contrast slightly
                intensity = pow(intensity, 1.1f);
                
                int idx = (y * cloudTexSize + x) * 3;
                unsigned char val = (unsigned char)(glm::clamp(intensity, 0.0f, 1.0f) * 255);
                cloudData[idx] = val;     // R
                cloudData[idx+1] = val;   // G
                cloudData[idx+2] = val;   // B
            }
        }
        cloudTexture = new Texture(cloudTexSize, cloudTexSize, cloudData, GL_RGB);
        delete[] cloudData;

        
        // cloudTexture = new Texture("../assets/textures/cloud.png"); // Replaced with procedural
        birdTexture = new Texture("../assets/textures/pigeon.png");
        // birdTexture->setFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST); // Reverted
        
        // Guard textures
        guardUniformTexture = new Texture("../assets/textures/guard_uniform.png");
        guardHelmetTexture = new Texture("../assets/textures/guard_helmet.png");
        guardBootsTexture = new Texture("../assets/textures/guard_boots.png");
        
        // Tree textures
        treeBarkTexture = new Texture("../assets/textures/tree_bark.png");
        treeLeavesTexture = new Texture("../assets/textures/tree_leaves.png");
        
        // ===== Create Objects (Assign to Global) =====
        // Base pavement (stone) - covers the whole area (Expanded)
        pavement = Primitives::createPlane(200.0f, 200.0f, 40.0f);
        
        // Sky Dome
        skyDome = Primitives::createSphere(1.0f, 32, 32);
        
        // Central pathway to Lang Bac entrance (stone)
        pathway = Primitives::createBox(12.0f, 0.02f, 60.0f);
        
        // Checkerboard Grass Field (Expanded to cover whole scene)
        // Create grass patches (10x10m) with gaps (1.5m)
        // DISABLED - No grass patches per user request
        float patchSize = 10.0f;
        float gap = 1.5f;
        float startZ = -90.0f;
        float endZ = 90.0f;
        float startX = -90.0f;
        float endX = 90.0f;
        
        sharedGrassPatch = Primitives::createBox(patchSize, 0.05f, patchSize);
        
        // Grass patches behind flagpole (Z > 40)
        // Grid pattern with paths between
        float grassStartZ = 45.0f; // Start behind flagpole (Flagpole is at Z=40)
        float grassEndZ = 95.0f;   // End before front fence (Z=100)
        float grassStartX = -95.0f;
        float grassEndX = 95.0f;
        
        // Increase gap to make paths more visible
        float currentGap = 2.5f; 
        
        for (float z = grassStartZ; z <= grassEndZ; z += (patchSize + currentGap))
        {
            for (float x = grassStartX; x <= grassEndX; x += (patchSize + currentGap))
            {
                // Leave a central path (Z-axis) - Wider for the main entrance
                if (x > -10.0f && x < 10.0f) continue;
                
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x, 0.05f, z));
                grassTransforms.push_back(model);
            }
        }
        
        langBac = new LangBac();
        cotCo = new CotCo(glm::vec3(0.0f, 0.0f, 40.0f)); // Moved closer to walkway (Z=40)

        // Street lights - Rewritten to be strictly on grass areas (Left & Right)
        // Grass Patch Centers Calculation:
        // Grid starts at X=-95, Step=12.5
        // Columns: -95, -82.5, -70, -57.5, -45, -32.5, -20 | GAP | 17.5, 30, 42.5, 55, 67.5, 80, 92.5
        // Rows (Z): 45, 57.5, 70, 82.5, 95
        
        // We place lights on:
        // 1. Inner columns (closest to central path): X = -20.0 and X = 17.5
        // 2. Middle-Inner columns: X = -51.25 and X = 48.75
        // 3. Outer columns (near perimeter): X = -82.5 and X = 80.0
        // Z positions: Start (45), Middle (70), End (95)
        
        float lightZ[] = {45.0f, 70.0f, 95.0f};
        
        for (float z : lightZ)
        {
            // Left side (3 rows)
            lights.push_back(new StreetLight(glm::vec3(-20.0f, 0.0f, z)));   // Inner Left
            lights.push_back(new StreetLight(glm::vec3(-51.25f, 0.0f, z)));  // Middle-Inner Left
            lights.push_back(new StreetLight(glm::vec3(-82.5f, 0.0f, z)));   // Outer Left
            
            // Right side (3 rows)
            lights.push_back(new StreetLight(glm::vec3(17.5f, 0.0f, z)));    // Inner Right
            lights.push_back(new StreetLight(glm::vec3(48.75f, 0.0f, z)));   // Middle-Inner Right
            lights.push_back(new StreetLight(glm::vec3(80.0f, 0.0f, z)));    // Outer Right
        }

        // Create Guards - Standing at attention at entrance
        guards.push_back(new Guard(glm::vec3(-6.0f, 0.0f, 0.0f), 180.0f)); // Left guard facing outward
        guards.push_back(new Guard(glm::vec3(6.0f, 0.0f, 0.0f), 180.0f));  // Right guard facing outward



        birds.push_back(new Bird(glm::vec3(0.0f, 20.0f, -10.0f), 30.0f, 0.0f));
        birds.push_back(new Bird(glm::vec3(0.0f, 25.0f, -10.0f), 35.0f, 3.14f));
        birds.push_back(new Bird(glm::vec3(0.0f, 18.0f, -10.0f), 25.0f, 1.57f));


        // Create Trees - All behind mausoleum and grandstands (Z < -15)
        // Left side trees (outer perimeter) - moved further left to avoid yard
        for (float z = -20.0f; z >= -50.0f; z -= 10.0f)
        {
            trees.push_back(new Tree(glm::vec3(-75.0f, 0.0f, z), 1.2f));
            trees.push_back(new Tree(glm::vec3(-85.0f, 0.0f, z), 1.3f));
            trees.push_back(new Tree(glm::vec3(-95.0f, 0.0f, z), 1.4f));
        }
        
        // Right side trees (outer perimeter) - moved further right to avoid yard
        for (float z = -20.0f; z >= -50.0f; z -= 10.0f)
        {
            trees.push_back(new Tree(glm::vec3(75.0f, 0.0f, z), 1.2f));
            trees.push_back(new Tree(glm::vec3(85.0f, 0.0f, z), 1.3f));
            trees.push_back(new Tree(glm::vec3(95.0f, 0.0f, z), 1.4f));
        }
        
        // Back row trees (behind mausoleum) - Fill gaps orderly
        // Rows at Z = -35, -45, -55 to create a dense background
        for (float z = -35.0f; z >= -55.0f; z -= 10.0f)
        {
            for (float x = -60.0f; x <= 60.0f; x += 10.0f)
            {
                // Add trees in a grid pattern
                trees.push_back(new Tree(glm::vec3(x, 0.0f, z), 1.5f));
            }
        }
        
        
        // Red flags along pathways - DISABLED per user request
        
        // Add fences along the perimeter of the yard (Front, Left, Right)
        // Yard is approx 200x200 (X: -100 to 100, Z: -100 to 100)
        // Front fence moved to Z=100 to occupy whole yard
        // Height increased to 3.0f
        fences.push_back(new Fence(glm::vec3(-100.0f, 0.0f, 100.0f), glm::vec3(100.0f, 0.0f, 100.0f), 3.0f));
        
        // Left fence (X=-100) - Extended back to Z=-60 to cover tree area
        fences.push_back(new Fence(glm::vec3(-100.0f, 0.0f, -60.0f), glm::vec3(-100.0f, 0.0f, 100.0f), 3.0f));
        
        // Right fence (X=100) - Extended back to Z=-60 to cover tree area
        fences.push_back(new Fence(glm::vec3(100.0f, 0.0f, -60.0f), glm::vec3(100.0f, 0.0f, 100.0f), 3.0f));
        /*
        // Left side flags (6 flags)
        for (int i = 0; i < 6; i++)
        {
            Mesh *flagPole = Primitives::createBox(0.1f, 4.0f, 0.1f);
            redFlags.push_back(flagPole);
            Mesh *flag = Primitives::createBox(1.5f, 1.0f, 0.05f);
            redFlags.push_back(flag);
        }
        
        // Right side flags (6 flags)
        for (int i = 0; i < 6; i++)
        {
            Mesh *flagPole = Primitives::createBox(0.1f, 4.0f, 0.1f);
            redFlags.push_back(flagPole);
            Mesh *flag = Primitives::createBox(1.5f, 1.0f, 0.05f);
            redFlags.push_back(flag);
        }
        */
        
        // Fences removed as requested

        
        // Create Sky Dome (Large Sphere)
        // We reuse Primitives::createSphere but will render it with specific shader
        
        // Create Clouds (High Altitude Volumetric Layers)
        // Layer 1: Very high, slow moving (Cirrus) - 280-350 height
        // Reduced count for performance with volumetric clouds
        for (int i = 0; i < 20; i++)
        {
            float x = -1500.0f + (rand() % 3000); // Range +/- 1500
            float z = -1500.0f + (rand() % 3000);
            float y = 280.0f + (rand() % 70); // Height 280-350
            float scale = 180.0f + (rand() % 100); // Larger scale 180-280
            float speed = 0.5f + (rand() % 10) / 10.0f; // Slow drift
            
            clouds.push_back(new Cloud(glm::vec3(x, y, z), speed, scale));
        }
        
        // Layer 2: Mid-high, slightly faster (Cumulus) - 200-260 height
        for (int i = 0; i < 35; i++)
        {
            float x = -1500.0f + (rand() % 3000); // Range +/- 1500
            float z = -1500.0f + (rand() % 3000);
            float y = 200.0f + (rand() % 60); // Height 200-260
            float scale = 120.0f + (rand() % 80); // Scale 120-200
            float speed = 1.5f + (rand() % 20) / 10.0f; 
            
            clouds.push_back(new Cloud(glm::vec3(x, y, z), speed, scale));
        }
        
        // Fences removed as requested

        
        // Create Background Buildings (City Skyline) - ONLY BACK
        // Layered approach: Mostly low-rise with few mid-rise buildings
        // Range: Z from -60 to -120
        // X from -100 to 100
        
        // Layer 1: Z = -70 (Low-rise buildings, 8-20m) - Most common
        for (float x = -100.0f; x <= 100.0f; x += 15.0f)
        {
            float w = 10.0f + (rand() % 8); // 10-18m width
            float h = 8.0f + (rand() % 12);  // 8-20m height (2-6 floors)
            float d = 10.0f + (rand() % 8);  // 10-18m depth
            
            // Use tiled box for better texture details
            Mesh *building = Primitives::createTiledBox(w, h, d, 0.5f);
            backgroundBuildings.push_back(building);
            
            glm::mat4 model = glm::mat4(1.0f);
            float zPos = -70.0f + (rand() % 10) - 5.0f;
            float xPos = x + (rand() % 8) - 4.0f;
            model = glm::translate(model, glm::vec3(xPos, h/2.0f, zPos));
            buildingTransforms.push_back(model);
        }
        
        // Layer 2: Z = -90 (Mid-rise buildings, 20-35m) - Less common
        // Fewer buildings, wider spacing
        for (float x = -100.0f; x <= 100.0f; x += 35.0f)
        {
            float w = 12.0f + (rand() % 8); // 12-20m width
            float h = 20.0f + (rand() % 15); // 20-35m height (6-10 floors)
            float d = 12.0f + (rand() % 8);  // 12-20m depth
            
            Mesh *building = Primitives::createTiledBox(w, h, d, 0.5f);
            backgroundBuildings.push_back(building);
            
            glm::mat4 model = glm::mat4(1.0f);
            float zPos = -90.0f + (rand() % 15) - 7.5f;
            float xPos = x + (rand() % 10) - 5.0f;
            model = glm::translate(model, glm::vec3(xPos, h/2.0f, zPos));
            buildingTransforms.push_back(model);
        }
        
        // Layer 3: Z = -120 (Occasional taller buildings, 35-50m) - Rare
        // Very few tall buildings, very wide spacing
        for (float x = -80.0f; x <= 80.0f; x += 60.0f)
        {
            float w = 15.0f + (rand() % 10); // 15-25m width
            float h = 35.0f + (rand() % 15); // 35-50m height (10-15 floors)
            float d = 15.0f + (rand() % 10); // 15-25m depth
            
            Mesh *building = Primitives::createTiledBox(w, h, d, 0.5f);
            backgroundBuildings.push_back(building);
            
            glm::mat4 model = glm::mat4(1.0f);
            float zPos = -120.0f + (rand() % 20) - 10.0f;
            float xPos = x + (rand() % 15) - 7.5f;
            model = glm::translate(model, glm::vec3(xPos, h/2.0f, zPos));
            buildingTransforms.push_back(model);
        }
        
        // Add Trees to Sides (Forest effect) - DISABLED to prevent clutter
        /*
        // Left Side (X < -60) - Only behind mausoleum
        for (float z = -100.0f; z <= -15.0f; z += 15.0f) // Only Z < -15
        {
             for (float x = -120.0f; x <= -60.0f; x += 15.0f)
             {
                 float scale = 1.0f + (rand() % 5) / 10.0f;
                 float xPos = x + (rand() % 5);
                 float zPos = z + (rand() % 5);
                 trees.push_back(new Tree(glm::vec3(xPos, 0.0f, zPos), scale));
             }
        }
        
        // Right Side (X > 60) - Only behind mausoleum
        for (float z = -100.0f; z <= -15.0f; z += 15.0f) // Only Z < -15
        {
             for (float x = 60.0f; x <= 120.0f; x += 15.0f)
             {
                 float scale = 1.0f + (rand() % 5) / 10.0f;
                 float xPos = x + (rand() % 5);
                 float zPos = z + (rand() % 5);
                 trees.push_back(new Tree(glm::vec3(xPos, 0.0f, zPos), scale));
             }
        }
        */
        
        // Text banners (red/yellow)
        Mesh *banner1 = Primitives::createBox(8.0f, 3.0f, 0.1f);
        textBanners.push_back(banner1);
        Mesh *banner2 = Primitives::createBox(8.0f, 3.0f, 0.1f);
        textBanners.push_back(banner2);

        TimeOfDay timeOfDay;

        // ===== SHADOW MAP FBO SETUP =====
        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
        unsigned int depthMapFBO;
        glGenFramebuffers(1, &depthMapFBO);
        
        unsigned int depthMap;
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // Shader configuration
        lightingShader.use();
        lightingShader.setInt("material.diffuse", 0);
        lightingShader.setInt("shadowMap", 1);

        std::cout << "Lang Bac scene V5.0 - Visual Polish & Guards loaded!" << std::endl;
        std::cout << "Controls: T = pause time, U = raise flag, L = lower flag" << std::endl;

        // ===== RENDER LOOP =====
        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            // T key toggle
            if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
                if (currentFrame - lastTKeyPress > 0.5f) {
                    timeOfDay.togglePause();
                    lastTKeyPress = currentFrame;
                }
            }
            // U key raise flag
            if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
                if (currentFrame - lastUKeyPress > 0.5f) {
                    cotCo->raiseFlag();
                    lastUKeyPress = currentFrame;
                    std::cout << "Raising flag..." << std::endl;
                }
            }
            // L key lower flag
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
                if (currentFrame - lastLKeyPress > 0.5f) {
                    cotCo->lowerFlag();
                    lastLKeyPress = currentFrame;
                    std::cout << "Lowering flag..." << std::endl;
                }
            }

            // Update
            timeOfDay.update(deltaTime);
            cotCo->update(deltaTime);
            for (auto guard : guards) guard->update(deltaTime);
            for (auto cloud : clouds) cloud->update(deltaTime);
            for (auto bird : birds) bird->update(deltaTime);

            // ====================================================
            // 1. Render depth of scene to texture (from light's perspective)
            // ====================================================
            glm::mat4 lightProjection, lightView;
            glm::mat4 lightSpaceMatrix;
            float near_plane = 1.0f, far_plane = 100.0f;
            
            glm::vec3 sunDir = timeOfDay.getSunDirection();
            glm::vec3 lightPos = sunDir * 50.0f;
            if (lightPos.y < 0) lightPos.y = 10.0f; 
            
            lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
            lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            lightSpaceMatrix = lightProjection * lightView;
            
            shadowShader.use();
            shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            RenderScene(shadowShader);
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // ====================================================
            // 2. Render scene as normal with shadow mapping
            // ====================================================
            glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
            glm::vec3 skyColor = timeOfDay.getSkyColor();
            
            // Increased far plane to 2000.0f for horizon-to-horizon visibility
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
            glm::mat4 view = camera.GetViewMatrix();

            // ===== RENDER SKY DOME =====
            if (skyShader && skyDome)
            {
                glDepthMask(GL_FALSE); // Don't write to depth buffer
                skyShader->use();
                skyShader->setMat4("projection", projection);
                skyShader->setMat4("view", view);
                
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, camera.Position); // Sky dome follows camera
                model = glm::scale(model, glm::vec3(200.0f)); // Large scale
                skyShader->setMat4("model", model);
                
                skyShader->setVec3("topColor", glm::vec3(0.1f, 0.3f, 0.7f)); // Realistic Deep Blue Zenith
                skyShader->setVec3("bottomColor", glm::vec3(0.7f, 0.8f, 0.9f)); // Hazy Horizon Blue
                skyShader->setFloat("time", currentFrame);
                skyShader->setBool("isNight", timeOfDay.isNightTime());
                
                // Cull front face because we are inside the sphere
                glCullFace(GL_FRONT);
                skyDome->draw();
                glCullFace(GL_BACK); // Reset
                
                glDepthMask(GL_TRUE);
            }
            
            glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            lightingShader.use();
            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);
            lightingShader.setVec3("viewPos", camera.Position);
            lightingShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
            
            lightingShader.setFloat("material.shininess", 32.0f);

            lightingShader.setVec3("dirLight.direction", sunDir);
            
            glm::vec3 sunColor = glm::vec3(1.0f);
            float ambientStrength = timeOfDay.getAmbientStrength();
            if (timeOfDay.isNightTime()) {
                sunColor = glm::vec3(0.2f, 0.2f, 0.3f); // Slightly brighter moonlight
                ambientStrength = 0.25f; // Increased from 0.1 for better visibility
            } else if (skyColor.r > 0.7f) {
                sunColor = glm::vec3(1.0f, 0.6f, 0.3f);
            }
            lightingShader.setVec3("dirLight.ambient", sunColor * ambientStrength);
            lightingShader.setVec3("dirLight.diffuse", sunColor * 0.8f);
            lightingShader.setVec3("dirLight.specular", sunColor * 0.5f);
            
            glm::vec3 streetLightColor = glm::vec3(0.0f);
            if (timeOfDay.isNightTime()) streetLightColor = glm::vec3(1.0f, 0.9f, 0.5f);
            
            // Use ALL 18 lights for complete coverage including middle area (Z=70)
            for (int i = 0; i < 18; i++) {
                std::string number = std::to_string(i);
                lightingShader.setVec3("pointLights[" + number + "].position", lights[i]->getLightPosition() + glm::vec3(0, -0.5f, 0));
                lightingShader.setVec3("pointLights[" + number + "].ambient", streetLightColor * 0.1f);
                lightingShader.setVec3("pointLights[" + number + "].diffuse", streetLightColor * 1.5f);
                lightingShader.setVec3("pointLights[" + number + "].specular", streetLightColor * 1.0f);
                lightingShader.setFloat("pointLights[" + number + "].constant", 1.0f);
                lightingShader.setFloat("pointLights[" + number + "].linear", 0.09f);
                lightingShader.setFloat("pointLights[" + number + "].quadratic", 0.032f);
            }
            
            lightingShader.setVec3("spotLight.position", cotCo->position + glm::vec3(0.0f, 0.5f, 2.0f));
            lightingShader.setVec3("spotLight.direction", glm::vec3(0.0f, 1.0f, -0.2f));
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.09f);
            lightingShader.setFloat("spotLight.quadratic", 0.032f);
            
            if (timeOfDay.isNightTime()) {
                lightingShader.setVec3("spotLight.ambient", glm::vec3(0.0f));
                lightingShader.setVec3("spotLight.diffuse", glm::vec3(1.0f));
                lightingShader.setVec3("spotLight.specular", glm::vec3(1.0f));
            } else {
                lightingShader.setVec3("spotLight.ambient", glm::vec3(0.0f));
                lightingShader.setVec3("spotLight.diffuse", glm::vec3(0.0f));
                lightingShader.setVec3("spotLight.specular", glm::vec3(0.0f));
            }

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            
            // Set uniforms for building window lights
            lightingShader.setBool("isNight", timeOfDay.isNightTime());
            lightingShader.setFloat("time", currentFrame);
            lightingShader.setBool("enableWindowLights", false); // Disable by default
            lightingShader.setBool("enableBulbGlow", false); // Will be enabled specifically for bulbs
            
            lightingShader.setFloat("material.shininess", 4.0f);
            RenderScene(lightingShader, timeOfDay.isNightTime());
            
            // ===== RENDER VOLUMETRIC CLOUDS =====
            if (cloudShader && cloudTexture)
            {
                cloudShader->use();
                cloudShader->setMat4("projection", projection);
                cloudShader->setMat4("view", view);
                cloudShader->setVec3("viewPos", camera.Position);
                cloudShader->setFloat("time", currentFrame);
                cloudShader->setInt("cloudTexture", 0);
                cloudShader->setVec3("skyColor", skyColor); // Pass sky color for time-based cloud coloring
                
                cloudTexture->bind(0);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDepthMask(GL_FALSE); // Disable depth writing for transparent clouds
                
                // Render each cloud's spheres
                for (auto cloud : clouds)
                {
                    int sphereCount = cloud->getSphereCount();
                    for (int i = 0; i < sphereCount; i++)
                    {
                        glm::mat4 modelCloud = cloud->getTransform(i);
                        cloudShader->setMat4("model", modelCloud);
                        cloud->spheres[i]->draw();
                    }
                }
                
                glDepthMask(GL_TRUE); // Re-enable depth writing
                glDisable(GL_BLEND);
            }
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        delete skyDome;
        delete skyShader;
        delete cloudShader;
        
        delete pavement;
        delete sharedGrassPatch;
        grassTransforms.clear();
        delete pathway;
        // grassGridLines removed
        delete langBac;
        delete cotCo;
        for (auto light : lights) delete light;
        for (auto guard : guards) delete guard;
        for (auto cloud : clouds) delete cloud;
        for (auto bird : birds) delete bird;
        
        delete grassTexture;
        delete stoneTexture;
        delete flagTexture;
        delete concreteTexture;
        delete metalTexture;
        delete cloudTexture;
        delete birdTexture;
        delete guardUniformTexture;
        delete guardHelmetTexture;
        delete guardBootsTexture;
        delete treeBarkTexture;
        delete treeLeavesTexture;

        // Duplicate deletions removed
        guards.clear();
        trees.clear();
        redFlags.clear();
        
        for (auto fence : fences) delete fence;
        fences.clear();
        
        for (auto building : backgroundBuildings) delete building;
        backgroundBuildings.clear();
        buildingTransforms.clear();
        
        for (auto banner : textBanners) delete banner;
        textBanners.clear();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    // Debug print to check if mouse is moving
    // std::cout << "Mouse move: " << xoffset << ", " << yoffset << std::endl;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        // Toggle time pause (if supported)
    }
    if (key == GLFW_KEY_U && action == GLFW_PRESS)
    {
        if (cotCo) cotCo->raiseFlag();
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (cotCo) cotCo->lowerFlag();
    }
}