#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

/**
 * 👤 NGƯỜI 2: Mesh (Representation of 3D geometry)
 * Lưu trữ và vẽ một mesh (tập hợp các đỉnh + indices)
 */

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
public:
    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint VAO, VBO, EBO;

    // Constructor
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
    ~Mesh();

    // Render the mesh
    void draw();

private:
    void setupMesh();
};

#endif
