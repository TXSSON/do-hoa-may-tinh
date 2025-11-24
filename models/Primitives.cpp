#include "Primitives.h"
#include <cmath>

/**
 * 👤 NGƯỜI 2: Primitives Factory Implementation
 * Tạo các hình học cơ bản (Plane, Box, Cylinder, Sphere)
 */

namespace Primitives
{
    Mesh *createPlane(float width, float depth, float tilingX, float tilingY)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    float halfWidth = width / 2.0f;
    float halfDepth = depth / 2.0f;
    
    // If tilingY is not specified (negative), use tilingX for uniform tiling
    if (tilingY < 0.0f) tilingY = tilingX;

    // Define 4 vertices for a quad (ground plane on XZ)
    // Bottom-left
    vertices.push_back({
        glm::vec3(-halfWidth, 0.0f, -halfDepth), // Position
        glm::vec3(0.0f, 1.0f, 0.0f),              // Normal (up)
        glm::vec2(0.0f, 0.0f)                     // TexCoord
    });

    // Bottom-right
    vertices.push_back({
        glm::vec3(halfWidth, 0.0f, -halfDepth),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec2(tilingX, 0.0f)});

    // Top-right
    vertices.push_back({
        glm::vec3(halfWidth, 0.0f, halfDepth),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec2(tilingX, tilingY)});

    // Top-left
    vertices.push_back({
        glm::vec3(-halfWidth, 0.0f, halfDepth),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec2(0.0f, tilingY)});

    // Define indices for 2 triangles (CCW winding)
    indices = {
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    return new Mesh(vertices, indices);
}
    Mesh *createBox(float width, float height, float depth)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        float w = width / 2.0f;
        float h = height / 2.0f;
        float d = depth / 2.0f;

        // We need 24 vertices (4 per face) for proper normals and UVs
        // Each face has its own vertices with correct normals

        // Front face (+Z)
        vertices.push_back({glm::vec3(-w, -h, d), glm::vec3(0, 0, 1), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(w, -h, d), glm::vec3(0, 0, 1), glm::vec2(1, 0)});
        vertices.push_back({glm::vec3(w, h, d), glm::vec3(0, 0, 1), glm::vec2(1, 1)});
        vertices.push_back({glm::vec3(-w, h, d), glm::vec3(0, 0, 1), glm::vec2(0, 1)});

        // Back face (-Z)
        vertices.push_back({glm::vec3(w, -h, -d), glm::vec3(0, 0, -1), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(-w, -h, -d), glm::vec3(0, 0, -1), glm::vec2(1, 0)});
        vertices.push_back({glm::vec3(-w, h, -d), glm::vec3(0, 0, -1), glm::vec2(1, 1)});
        vertices.push_back({glm::vec3(w, h, -d), glm::vec3(0, 0, -1), glm::vec2(0, 1)});

        // Left face (-X)
        vertices.push_back({glm::vec3(-w, -h, -d), glm::vec3(-1, 0, 0), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(-w, -h, d), glm::vec3(-1, 0, 0), glm::vec2(1, 0)});
        vertices.push_back({glm::vec3(-w, h, d), glm::vec3(-1, 0, 0), glm::vec2(1, 1)});
        vertices.push_back({glm::vec3(-w, h, -d), glm::vec3(-1, 0, 0), glm::vec2(0, 1)});

        // Right face (+X)
        vertices.push_back({glm::vec3(w, -h, d), glm::vec3(1, 0, 0), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(w, -h, -d), glm::vec3(1, 0, 0), glm::vec2(1, 0)});
        vertices.push_back({glm::vec3(w, h, -d), glm::vec3(1, 0, 0), glm::vec2(1, 1)});
        vertices.push_back({glm::vec3(w, h, d), glm::vec3(1, 0, 0), glm::vec2(0, 1)});

        // Top face (+Y)
        vertices.push_back({glm::vec3(-w, h, d), glm::vec3(0, 1, 0), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(w, h, d), glm::vec3(0, 1, 0), glm::vec2(1, 0)});
        vertices.push_back({glm::vec3(w, h, -d), glm::vec3(0, 1, 0), glm::vec2(1, 1)});
        vertices.push_back({glm::vec3(-w, h, -d), glm::vec3(0, 1, 0), glm::vec2(0, 1)});

        // Bottom face (-Y)
        vertices.push_back({glm::vec3(-w, -h, -d), glm::vec3(0, -1, 0), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(w, -h, -d), glm::vec3(0, -1, 0), glm::vec2(1, 0)});
        vertices.push_back({glm::vec3(w, -h, d), glm::vec3(0, -1, 0), glm::vec2(1, 1)});
        vertices.push_back({glm::vec3(-w, -h, d), glm::vec3(0, -1, 0), glm::vec2(0, 1)});

        // Generate indices for 6 faces (2 triangles per face)
        for (int i = 0; i < 6; i++)
        {
            int offset = i * 4;
            indices.push_back(offset + 0);
            indices.push_back(offset + 1);
            indices.push_back(offset + 2);
            indices.push_back(offset + 2);
            indices.push_back(offset + 3);
            indices.push_back(offset + 0);
        }

        return new Mesh(vertices, indices);
    }

    Mesh *createTiledBox(float width, float height, float depth, float tileScale)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        float w = width / 2.0f;
        float h = height / 2.0f;
        float d = depth / 2.0f;
        
        // Calculate UV tiling based on dimensions and scale
        float uX = width * tileScale;
        float uY = height * tileScale;
        float uZ = depth * tileScale;

        // Front face (+Z)
        vertices.push_back({glm::vec3(-w, -h, d), glm::vec3(0, 0, 1), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(w, -h, d), glm::vec3(0, 0, 1), glm::vec2(uX, 0)});
        vertices.push_back({glm::vec3(w, h, d), glm::vec3(0, 0, 1), glm::vec2(uX, uY)});
        vertices.push_back({glm::vec3(-w, h, d), glm::vec3(0, 0, 1), glm::vec2(0, uY)});

        // Back face (-Z)
        vertices.push_back({glm::vec3(w, -h, -d), glm::vec3(0, 0, -1), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(-w, -h, -d), glm::vec3(0, 0, -1), glm::vec2(uX, 0)});
        vertices.push_back({glm::vec3(-w, h, -d), glm::vec3(0, 0, -1), glm::vec2(uX, uY)});
        vertices.push_back({glm::vec3(w, h, -d), glm::vec3(0, 0, -1), glm::vec2(0, uY)});

        // Left face (-X)
        vertices.push_back({glm::vec3(-w, -h, -d), glm::vec3(-1, 0, 0), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(-w, -h, d), glm::vec3(-1, 0, 0), glm::vec2(uZ, 0)});
        vertices.push_back({glm::vec3(-w, h, d), glm::vec3(-1, 0, 0), glm::vec2(uZ, uY)});
        vertices.push_back({glm::vec3(-w, h, -d), glm::vec3(-1, 0, 0), glm::vec2(0, uY)});

        // Right face (+X)
        vertices.push_back({glm::vec3(w, -h, d), glm::vec3(1, 0, 0), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(w, -h, -d), glm::vec3(1, 0, 0), glm::vec2(uZ, 0)});
        vertices.push_back({glm::vec3(w, h, -d), glm::vec3(1, 0, 0), glm::vec2(uZ, uY)});
        vertices.push_back({glm::vec3(w, h, d), glm::vec3(1, 0, 0), glm::vec2(0, uY)});

        // Top face (+Y)
        vertices.push_back({glm::vec3(-w, h, d), glm::vec3(0, 1, 0), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(w, h, d), glm::vec3(0, 1, 0), glm::vec2(uX, 0)});
        vertices.push_back({glm::vec3(w, h, -d), glm::vec3(0, 1, 0), glm::vec2(uX, uZ)});
        vertices.push_back({glm::vec3(-w, h, -d), glm::vec3(0, 1, 0), glm::vec2(0, uZ)});

        // Bottom face (-Y)
        vertices.push_back({glm::vec3(-w, -h, -d), glm::vec3(0, -1, 0), glm::vec2(0, 0)});
        vertices.push_back({glm::vec3(w, -h, -d), glm::vec3(0, -1, 0), glm::vec2(uX, 0)});
        vertices.push_back({glm::vec3(w, -h, d), glm::vec3(0, -1, 0), glm::vec2(uX, uZ)});
        vertices.push_back({glm::vec3(-w, -h, d), glm::vec3(0, -1, 0), glm::vec2(0, uZ)});

        // Generate indices for 6 faces (2 triangles per face)
        for (int i = 0; i < 6; i++)
        {
            int offset = i * 4;
            indices.push_back(offset + 0);
            indices.push_back(offset + 1);
            indices.push_back(offset + 2);
            indices.push_back(offset + 2);
            indices.push_back(offset + 3);
            indices.push_back(offset + 0);
        }

        return new Mesh(vertices, indices);
    }

    Mesh *createSphere(float radius, int sectorCount, int stackCount)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        float x, y, z, xy;
        float nx, ny, nz, lengthInv = 1.0f / radius;
        float s, t;

        float sectorStep = 2 * M_PI / sectorCount;
        float stackStep = M_PI / stackCount;
        float sectorAngle, stackAngle;

        // Generate vertices
        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = M_PI / 2 - i * stackStep;
            xy = radius * cosf(stackAngle);
            z = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;

                x = xy * cosf(sectorAngle);
                y = xy * sinf(sectorAngle);

                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;

                s = (float)j / sectorCount;
                t = (float)i / stackCount;

                vertices.push_back({
                    glm::vec3(x, y, z),
                    glm::vec3(nx, ny, nz),
                    glm::vec2(s, t)});
            }
        }

        // Generate indices
        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        return new Mesh(vertices, indices);
    }

    Mesh *createCylinder(float radius, float height, int segments)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        float halfHeight = height / 2.0f;
        float angleStep = 2.0f * M_PI / segments;

        // Top and bottom centers
        int topCenterIdx = vertices.size();
        vertices.push_back({glm::vec3(0, halfHeight, 0), glm::vec3(0, 1, 0), glm::vec2(0.5f, 0.5f)});

        int bottomCenterIdx = vertices.size();
        vertices.push_back({glm::vec3(0, -halfHeight, 0), glm::vec3(0, -1, 0), glm::vec2(0.5f, 0.5f)});

        // Generate side vertices
        for (int i = 0; i <= segments; i++)
        {
            float angle = i * angleStep;
            float x = radius * cosf(angle);
            float z = radius * sinf(angle);
            float u = (float)i / segments;

            // Top ring
            vertices.push_back({
                glm::vec3(x, halfHeight, z),
                glm::vec3(x / radius, 0, z / radius),
                glm::vec2(u, 1)});

            // Bottom ring
            vertices.push_back({
                glm::vec3(x, -halfHeight, z),
                glm::vec3(x / radius, 0, z / radius),
                glm::vec2(u, 0)});
        }

        // Generate indices for side
        int baseIdx = 2;
        for (int i = 0; i < segments; i++)
        {
            int current = baseIdx + i * 2;
            int next = baseIdx + ((i + 1) % (segments + 1)) * 2;

            // Side quad (2 triangles)
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            indices.push_back(next);
            indices.push_back(next + 1);
            indices.push_back(current + 1);

            // Top cap
            indices.push_back(topCenterIdx);
            indices.push_back(current);
            indices.push_back(next);

            // Bottom cap
            indices.push_back(bottomCenterIdx);
            indices.push_back(next + 1);
            indices.push_back(current + 1);
        }

        return new Mesh(vertices, indices);
    }
}
