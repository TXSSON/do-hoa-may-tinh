# 🏗️ Cấu Trúc Dự Án OpenGL - Lăng Bác

## 📁 Cấu trúc thư mục

```
detaidohoa/
├── CMakeLists.txt              # Build configuration
├── main.cpp                     # Entry point - tích hợp 3 module
│
├── core/                        # 👤 NGƯỜI 1: Scene & Camera
│   ├── Camera.h/cpp            
│   ├── Scene.h/cpp             # Quản lý scene (tọa độ objects, background)
│   └── Input.h/cpp             # Xử lý input (keyboard, mouse)
│
├── rendering/                   # 👤 NGƯỜI 3: Lighting & Shaders
│   ├── Shader.h/cpp            
│   ├── Light.h/cpp             # Directional/Point/Spot lights
│   └── Material.h/cpp          # Phong/PBR materials
│
├── models/                      # 👤 NGƯỜI 2: Models & Textures
│   ├── Mesh.h/cpp              # Đại diện 1 mesh (VAO/VBO/EBO)
│   ├── Model.h/cpp             # Load model từ file (hoặc code)
│   ├── Texture.h/cpp           # Load & bind textures
│   └── Primitives.h/cpp        # Plane, Box, Cylinder, Sphere
│
├── objects/                     # Các đối tượng cụ thể (dùng chung)
│   ├── Lang.h/cpp              # Mô hình Lăng Bác
│   ├── CotCo.h/cpp             # Cột cờ
│   ├── Ground.h/cpp            # Nền đất
│   └── Tree.h/cpp              # Cây cối
│
├── shaders/                     # GLSL shader files
│   ├── basic.vs/.fs            # Shader cơ bản (màu)
│   ├── texture.vs/.fs          # Shader có texture
│   ├── lighting.vs/.fs         # Shader có ánh sáng (Phong)
│   └── shadow.vs/.fs           # Shadow mapping (nâng cao)
│
├── assets/                      # Tài nguyên
│   ├── textures/               # File ảnh (.png, .jpg)
│   │   ├── brick.jpg
│   │   ├── grass.jpg
│   │   └── marble.jpg
│   └── models/                 # File 3D (.obj, .fbx) nếu có
│
├── glad/                        # OpenGL loader
│   ├── include/
│   └── src/
│
└── build/                       # CMake build output (gitignore)
```

---

## 👥 Phân chia chi tiết

### 👤 NGƯỜI 1: Core System (Scene & Camera)

**Các file phụ trách:**
- `core/Camera.h/cpp` ✅ (đã có)
- `core/Scene.h/cpp` (mới)
- `core/Input.h/cpp` (mới)
- `main.cpp` (phần khởi tạo window, render loop)

**Nhiệm vụ tuần 1:**
1. ✅ Tạo cửa sổ OpenGL (đã xong)
2. ✅ Implement Camera với WASD + Mouse (đã xong)
3. 🔲 Tạo class `Scene` để quản lý danh sách objects
4. 🔲 Gửi ma trận View/Projection cho shader

**Code mẫu Scene.h:**
```cpp
class Scene {
    Camera camera;
    std::vector<Object*> objects;
public:
    void addObject(Object* obj);
    void render(Shader& shader);
    Camera& getCamera() { return camera; }
};
```

---

### 👤 NGƯỜI 2: Models & Textures

**Các file phụ trách:**
- `models/Mesh.h/cpp` (mới)
- `models/Primitives.h/cpp` (mới)
- `models/Texture.h/cpp` (mới)
- `assets/textures/` (tìm/tải texture)

**Nhiệm vụ tuần 1:**
1. 🔲 Viết class `Texture` để load ảnh (dùng stb_image)
2. 🔲 Viết class `Mesh` để lưu VAO/VBO
3. 🔲 Viết `Primitives::createPlane()` (mặt phẳng)
4. 🔲 Viết `Primitives::createBox()` (hình hộp)

**Code mẫu Mesh.h:**
```cpp
class Mesh {
    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
public:
    Mesh(std::vector<Vertex> verts, std::vector<GLuint> idx);
    void draw();
};
```

---

### 👤 NGƯỜI 3: Lighting & Shaders

**Các file phụ trách:**
- `rendering/Shader.h/cpp` ✅ (đã có)
- `rendering/Light.h/cpp` (mới)
- `rendering/Material.h/cpp` (mới)
- `shaders/*.vs, *.fs` (GLSL files)

**Nhiệm vụ tuần 1:**
1. ✅ Basic shader (đã có test.vs/fs)
2. 🔲 Viết `lighting.vs/fs` với Phong shading
3. 🔲 Tạo class `Light` (direction, color, intensity)
4. 🔲 Gửi uniform light parameters cho shader

**Code mẫu Light.h:**
```cpp
struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    
    void sendToShader(Shader& shader);
};
```

---

## 🔗 Điểm tích hợp

### Tuần 1 - Milestone: "First Triangle with Light"
```cpp
// main.cpp - Tích hợp cả 3 người
Scene scene;                          // Người 1
Mesh* plane = Primitives::createPlane(); // Người 2
Texture grass("assets/grass.jpg");    // Người 2
Shader litShader("lighting.vs", "lighting.fs"); // Người 3
DirectionalLight sun;                 // Người 3

while (!glfwWindowShouldClose(window)) {
    // Người 1: Camera matrices
    litShader.setMat4("view", scene.getCamera().GetViewMatrix());
    litShader.setMat4("projection", scene.getCamera().GetProjection());
    
    // Người 3: Light
    sun.sendToShader(litShader);
    
    // Người 2: Draw
    grass.bind();
    plane->draw();
}
```

---

## 📝 Quy tắc làm việc

### Git Workflow
1. **Mỗi người tạo branch riêng:**
   - `feature/camera-system` (Người 1)
   - `feature/texture-system` (Người 2)
   - `feature/lighting` (Người 3)

2. **Commit thường xuyên:**
   ```bash
   git add core/Camera.cpp
   git commit -m "Add camera WASD movement"
   git push origin feature/camera-system
   ```

3. **Merge khi hoàn thành milestone:**
   - Tạo Pull Request
   - Review code
   - Merge vào `main`

### Header Guards
```cpp
#ifndef CAMERA_H
#define CAMERA_H
// ... code ...
#endif
```

### Namespace (optional)
```cpp
namespace DoAnDoHoa {
    class Camera { ... };
}
```

---

## 🎯 Timeline 3 tuần

| Tuần | Người 1 | Người 2 | Người 3 |
|------|---------|---------|---------|
| **1** | Camera + Scene | Plane + Box + Texture | Phong Lighting |
| **2** | Skybox | Model Lăng + Cột | Shadow Mapping |
| **3** | Polish UI | Trees + Details | Advanced Effects |

---

## 🚀 Bắt đầu ngay

### 1. Tạo cấu trúc thư mục
```bash
mkdir -p core rendering models objects shaders assets/textures assets/models
```

### 2. Di chuyển file hiện tại
```bash
mv Camera.h core/
mv Shader.h rendering/
mv test.vs test.fs shaders/
```

### 3. Mỗi người tạo branch
```bash
git checkout -b feature/[tên-module]
```

### 4. Bắt đầu code!
