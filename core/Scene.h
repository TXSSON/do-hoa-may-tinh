#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Camera.h"

// Forward declarations
class Shader;
class Object;

/**
 * 👤 NGƯỜI 1: Scene Manager
 * Quản lý camera và danh sách các objects trong scene
 */
class Scene
{
public:
    Camera camera;

    Scene();
    Scene(glm::vec3 cameraPos);

    // Camera access
    Camera &getCamera() { return camera; }

    // Object management
    void addObject(Object *obj);
    void removeObject(Object *obj);
    void clear();

    // Render all objects in scene
    void render(Shader &shader);

    // Background color
    void setBackgroundColor(float r, float g, float b);
    glm::vec3 getBackgroundColor() const { return backgroundColor; }

private:
    std::vector<Object *> objects;
    glm::vec3 backgroundColor;
};

#endif
