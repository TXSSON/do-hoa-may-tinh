#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Mesh.h"

/**
 * 👤 NGƯỜI 2: Primitives Factory
 * Tạo các hình học cơ bản (Plane, Box, Cylinder, Sphere)
 */
namespace Primitives
{
    /**
     * Tạo một mặt phẳng nằm ngang (ground)
     * @param width Độ rộng
     * @param depth Độ sâu
     * @return Con trỏ tới Mesh đã tạo
     */
    Mesh *createPlane(float width = 10.0f, float depth = 10.0f, float tilingX = 1.0f, float tilingY = -1.0f);

    /**
     * Tạo một hình hộp
     * @param width Chiều rộng (X)
     * @param height Chiều cao (Y)
     * @param depth Chiều sâu (Z)
     * @return Con trỏ tới Mesh đã tạo
     */
    Mesh *createBox(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
    Mesh *createTiledBox(float width, float height, float depth, float tileScale);

    /**
     * Tạo hình cầu (cho cây, cột đèn...)
     */
    Mesh *createSphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18);

    /**
     * Tạo hình trụ (cho cột, thân cây...)
     */
    Mesh *createCylinder(float radius = 0.5f, float height = 2.0f, int segments = 36);
}

#endif
