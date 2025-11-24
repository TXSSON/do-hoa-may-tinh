# 🚀 Quick Start Guide

## Bắt đầu cho từng người

### 👤 NGƯỜI 1 - Bắt đầu ngay
Bạn đã có Camera hoạt động! Nhiệm vụ tiếp theo:

1. **Implement Scene.cpp**:
```bash
touch core/Scene.cpp
```

2. **Code cơ bản** (xem PROJECT_STRUCTURE.md để biết chi tiết)

3. **Test**: Chạy chương trình, thử WASD + chuột

---

### 👤 NGƯỜI 2 - Cần cài thư viện
Trước khi code, cần download **stb_image.h** để load texture:

```bash
cd models/
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
```

Hoặc tải thủ công từ: https://github.com/nothings/stb/blob/master/stb_image.h

Sau đó bắt đầu với:
```bash
touch models/Mesh.cpp models/Texture.cpp models/Primitives.cpp
```

---

### 👤 NGƯỜI 3 - Shader đầu tiên
Bạn đã có `lighting.vs` và `lighting.fs`! 

1. **Implement Light.cpp**:
```bash
touch rendering/Light.cpp
```

2. **Test shader**: Thử compile shader trong main.cpp
```cpp
Shader litShader("shaders/lighting.vs", "shaders/lighting.fs");
```

---

## Build & Run

```bash
cd build/
cmake ..
make
./DoAnApp
```

## Git Workflow

### Tạo branch riêng cho từng người:
```bash
# Người 1
git checkout -b feature/camera-scene

# Người 2
git checkout -b feature/models-textures

# Người 3
git checkout -b feature/lighting-shaders
```

### Commit thường xuyên:
```bash
git add core/Scene.cpp
git commit -m "Add Scene class with object management"
git push origin feature/camera-scene
```

### Merge khi xong milestone:
1. Tạo Pull Request trên GitHub
2. Review code lẫn nhau
3. Merge vào `main`

---

## Tài liệu tham khảo

- **LearnOpenGL**: https://learnopengl.com/
  - Camera: https://learnopengl.com/Getting-started/Camera
  - Textures: https://learnopengl.com/Getting-started/Textures
  - Lighting: https://learnopengl.com/Lighting/Basic-Lighting

- **GLM Documentation**: https://glm.g-truc.net/

---

## Liên hệ & Hỗ trợ

Nếu gặp vấn đề, hỏi trong group chat hoặc tạo GitHub Issue!

**Chúc các bạn code vui vẻ! 🎉**
