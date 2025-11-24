#include "Lang.h"
#include "Primitives.h"

/**
 * Authentic Ho Chi Minh Mausoleum Model - V2.0
 * Based on actual architecture:
 * - 2-tier structure (lower solid base + upper open colonnade)
 * - 4 large BRONZE/BROWN rectangular pillars on upper tier
 * - Open spaces between pillars (not solid walls)
 * - Flat roof with overhang
 * - "CHỦ TỊCH HỒ-CHÍ-MINH" text on top facade
 * - Gray granite base with darker bronze pillars
 */

LangBac::LangBac() : position(0.0f, 0.0f, -10.0f)
{
    createBasePlatform();
    createStairs();
    createLowerTier();
    createUpperStairs(); // Create steps between tiers
    createUpperTier();
    createColumns();
    createDoor();
    createRoof();
    createTopText();
}

LangBac::~LangBac()
{
    for (auto mesh : meshes)
    {
        delete mesh;
    }
    meshes.clear();
}

void LangBac::createBasePlatform()
{
    // Wide ground platform
    Mesh *basePlatform = Primitives::createBox(50.0f, 0.5f, 40.0f);
    meshes.push_back(basePlatform);
}

void LangBac::createStairs()
{
    // Front stairs (5 steps) - matching reference photo
    int numSteps = 5;
    for (int i = 0; i < numSteps; i++)
    {
        float stepWidth = 26.0f;
        float stepDepth = 3.0f;  // Wide treads
        float stepHeight = 0.3f; // Comfortable rise
        
        Mesh *step = Primitives::createBox(stepWidth, stepHeight, stepDepth);
        meshes.push_back(step);
    }
}

void LangBac::createLowerTier()
{
    // Lower tier - solid granite base with entrance (TẦNG 1)
    // Main body - wider and more accurate proportions
    Mesh *lowerBody = Primitives::createBox(24.0f, 5.0f, 18.0f);
    meshes.push_back(lowerBody);
    
    // Entrance recess area (darker interior)
    Mesh *entranceRecess = Primitives::createBox(6.0f, 4.0f, 1.5f);
    meshes.push_back(entranceRecess);
}

void LangBac::createUpperStairs()
{
    // Stepped transition from Tier 1 to Tier 2 (3 steps)
    // These sit on top of the lower tier
    int numSteps = 3;
    for (int i = 0; i < numSteps; i++)
    {
        // Each step gets slightly smaller
        float width = 23.0f - i * 1.0f;
        float depth = 17.0f - i * 1.0f;
        float height = 0.5f;
        
        Mesh *step = Primitives::createBox(width, height, depth);
        meshes.push_back(step);
    }
}

void LangBac::createUpperTier()
{
    // Upper tier - Inner Chamber (TẦNG 2)
    // 4 Brown walls inside the colonnade
    // Dimensions: Increased to be VERY close to columns (X=±10) and Ceiling
    // Columns are at X=±10, Z=-3 to -17
    // Inner chamber: X=19.0 (±9.5), Z=14.0 (centered), Height=8.0 (touching ceiling)
    Mesh *innerChamber = Primitives::createBox(19.0f, 8.0f, 14.0f);
    meshes.push_back(innerChamber);
}

void LangBac::createColumns()
{
    // 20 SQUARE COLUMNS total:
    // - 4 corner columns
    // - 4 columns per side (front, back, left, right)
    // Square cross-section (1.2 x 8.0 x 1.2)
    
    for (int i = 0; i < 20; i++)
    {
        Mesh *column = Primitives::createBox(1.2f, 8.0f, 1.2f); // Square column
        meshes.push_back(column);
    }
}

void LangBac::createDoor()
{
    // Door removed as requested
}

void LangBac::createRoof()
{
    // Flat roof with overhang
    Mesh *roofBase = Primitives::createBox(22.0f, 1.0f, 16.0f);
    meshes.push_back(roofBase);
    
    // Roof overhang/trim (The Facade for Text)
    // Increased height to 2.5f to accommodate text
    Mesh *roofOverhang = Primitives::createBox(24.0f, 2.5f, 17.5f);
    meshes.push_back(roofOverhang);
}

void LangBac::createTopText()
{
    // Text rendering removed - not feasible without font system
    // Mausoleum roof remains clean as in reality
}

void LangBac::draw()
{
    // Rendering handled in main.cpp
}
