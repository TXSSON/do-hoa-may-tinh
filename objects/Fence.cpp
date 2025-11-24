#include "Fence.h"
#include "Primitives.h"

Fence::Fence(glm::vec3 start, glm::vec3 end, float h)
    : startPos(start), endPos(end), height(h)
{
    createFenceSegments();
    createSquarePatterns();
}

Fence::~Fence()
{
    for (auto seg : segments) delete seg;
    for (auto pat : patterns) delete pat;
    segments.clear();
    patterns.clear();
}

void Fence::createFenceSegments()
{
    // Horizontal bars (top and bottom)
    float length = glm::length(endPos - startPos);
    
    // Top bar
    Mesh *topBar = Primitives::createBox(length, 0.1f, 0.1f);
    segments.push_back(topBar);
    
    // Bottom bar
    Mesh *bottomBar = Primitives::createBox(length, 0.1f, 0.1f);
    segments.push_back(bottomBar);
    
    // Vertical posts every 2 meters
    int numPosts = (int)(length / 2.0f) + 1;
    for (int i = 0; i < numPosts; i++)
    {
        Mesh *post = Primitives::createBox(0.1f, height, 0.1f);
        segments.push_back(post);
    }
}

void Fence::createSquarePatterns()
{
    // Traditional square patterns between posts
    float length = glm::length(endPos - startPos);
    int numSections = (int)(length / 2.0f);
    
    // Scale factors based on height (assuming default height was ~1.5)
    float scale = height / 1.5f; 
    
    for (int i = 0; i < numSections; i++)
    {
        // Outer square
        Mesh *outerSquare = Primitives::createBox(1.5f * scale, 0.05f * scale, 0.05f * scale); // Horizontal
        patterns.push_back(outerSquare);
        Mesh *outerSquare2 = Primitives::createBox(1.5f * scale, 0.05f * scale, 0.05f * scale); // Horizontal
        patterns.push_back(outerSquare2);
        Mesh *outerSquare3 = Primitives::createBox(0.05f * scale, 0.7f * scale, 0.05f * scale); // Vertical
        patterns.push_back(outerSquare3);
        Mesh *outerSquare4 = Primitives::createBox(0.05f * scale, 0.7f * scale, 0.05f * scale); // Vertical
        patterns.push_back(outerSquare4);
        
        // Inner square (smaller)
        Mesh *innerSquare = Primitives::createBox(0.8f * scale, 0.05f * scale, 0.05f * scale); // Horizontal
        patterns.push_back(innerSquare);
        Mesh *innerSquare2 = Primitives::createBox(0.8f * scale, 0.05f * scale, 0.05f * scale); // Horizontal
        patterns.push_back(innerSquare2);
        Mesh *innerSquare3 = Primitives::createBox(0.05f * scale, 0.4f * scale, 0.05f * scale); // Vertical
        patterns.push_back(innerSquare3);
        Mesh *innerSquare4 = Primitives::createBox(0.05f * scale, 0.4f * scale, 0.05f * scale); // Vertical
        patterns.push_back(innerSquare4);
    }
}

void Fence::draw(Shader &shader)
{
    glm::vec3 direction = glm::normalize(endPos - startPos);
    float length = glm::length(endPos - startPos);
    // Object is X-aligned. We need angle from X-axis.
    // In GL, +Y rotation is CCW (X -> -Z).
    // We want to rotate X to direction.
    float angle = -atan2(direction.z, direction.x);
    
    // Center position of the fence
    glm::vec3 center = (startPos + endPos) * 0.5f;
    
    // Scale factors based on height
    float scale = height / 1.5f;
    
    // Draw segments
    // Top bar (index 0)
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, center + glm::vec3(0.0f, height, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    shader.setMat4("model", model);
    segments[0]->draw();
    
    // Bottom bar (index 1)
    model = glm::mat4(1.0f);
    model = glm::translate(model, center + glm::vec3(0.0f, 0.1f, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    shader.setMat4("model", model);
    segments[1]->draw();
    
    // Posts (index 2 onwards)
    int numPosts = segments.size() - 2;
    for (int i = 0; i < numPosts; i++)
    {
        float t = (float)i / (numPosts - 1);
        glm::vec3 pos = startPos + direction * (t * length);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, pos + glm::vec3(0.0f, height * 0.5f, 0.0f));
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        segments[2 + i]->draw();
    }
    
    // Draw patterns
    int numSections = patterns.size() / 8; // 8 meshes per pattern
    float sectionLength = 2.0f;
    
    for (int i = 0; i < numSections; i++)
    {
        // Calculate center of this section
        float dist = sectionLength * (i + 0.5f);
        if (dist > length) break;
        
        glm::vec3 pos = startPos + direction * dist;
        
        // 1. Outer Square Top
        model = glm::mat4(1.0f);
        model = glm::translate(model, pos + glm::vec3(0.0f, height - 0.15f * scale, 0.0f));
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        patterns[i*8 + 0]->draw();
        
        // 2. Outer Square Bottom
        model = glm::mat4(1.0f);
        model = glm::translate(model, pos + glm::vec3(0.0f, 0.1f + 0.15f * scale, 0.0f)); // slightly above bottom bar
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        patterns[i*8 + 1]->draw();
        
        // 3. Outer Square Left
        model = glm::mat4(1.0f);
        model = glm::translate(model, pos + glm::vec3(0.0f, height * 0.5f, 0.0f));
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-0.75f * scale, 0.0f, 0.0f));
        shader.setMat4("model", model);
        patterns[i*8 + 2]->draw();
        
        // 4. Outer Square Right
        model = glm::mat4(1.0f);
        model = glm::translate(model, pos + glm::vec3(0.0f, height * 0.5f, 0.0f));
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.75f * scale, 0.0f, 0.0f));
        shader.setMat4("model", model);
        patterns[i*8 + 3]->draw();
        
        // Inner Square (Similar logic but smaller)
        // 5. Inner Top
        model = glm::mat4(1.0f);
        model = glm::translate(model, pos + glm::vec3(0.0f, height - 0.3f * scale, 0.0f));
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        patterns[i*8 + 4]->draw();
        
        // 6. Inner Bottom
        model = glm::mat4(1.0f);
        model = glm::translate(model, pos + glm::vec3(0.0f, 0.1f + 0.3f * scale, 0.0f));
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        patterns[i*8 + 5]->draw();
        
        // 7. Inner Left
        model = glm::mat4(1.0f);
        model = glm::translate(model, pos + glm::vec3(0.0f, height * 0.5f, 0.0f));
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-0.4f * scale, 0.0f, 0.0f));
        shader.setMat4("model", model);
        patterns[i*8 + 6]->draw();
        
        // 8. Inner Right
        model = glm::mat4(1.0f);
        model = glm::translate(model, pos + glm::vec3(0.0f, height * 0.5f, 0.0f));
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.4f * scale, 0.0f, 0.0f));
        shader.setMat4("model", model);
        patterns[i*8 + 7]->draw();
    }
}
