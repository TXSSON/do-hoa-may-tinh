#ifndef LANG_H
#define LANG_H

#include "Mesh.h"
#include <vector>
#include <glm/glm.hpp>

/**
 * Mô hình Lăng Bác (Ho Chi Minh Mausoleum)
 * Simplified version using primitives
 */
class LangBac
{
public:
    std::vector<Mesh *> meshes;

    LangBac();
    ~LangBac();

    void draw();
    glm::vec3 getPosition() { return position; }

private:
    glm::vec3 position;
    void createBasePlatform();
    void createStairs();
    void createLowerTier();
    void createUpperStairs(); // New stepped transition
    void createUpperTier();
    void createColumns();
    void createDoor();
    void createRoof();
    void createTopText();
};

#endif
