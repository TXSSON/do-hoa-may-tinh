#include "Mesh.h"
#include <iostream>

/**
 * 👤 NGƯỜI 2: Mesh Implementation
 * Quản lý OpenGL buffers (VAO/VBO/EBO) và render geometry
 */

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
    : vertices(vertices), indices(indices), VAO(0), VBO(0), EBO(0)
{
    setupMesh();
}

Mesh::~Mesh()
{
    // Cleanup OpenGL resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (!indices.empty())
    {
        glDeleteBuffers(1, &EBO);
    }
}

void Mesh::setupMesh()
{
    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // Load vertex data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Setup EBO if we have indices
    if (!indices.empty())
    {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    }

    // Vertex Positions (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    // Vertex Normals (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

    // Vertex Texture Coords (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    // Unbind VAO
    glBindVertexArray(0);
}

void Mesh::draw()
{
    // Bind VAO
    glBindVertexArray(VAO);

    // Draw mesh
    if (!indices.empty())
    {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

    // Unbind VAO
    glBindVertexArray(0);
}
