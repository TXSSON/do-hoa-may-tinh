# 🏛️ Dự án Lăng Bác - OpenGL

Dự án mô phỏng 3D Lăng Chủ tịch Hồ Chí Minh sử dụng OpenGL.

## 👥 Thành viên

- **Người 1**: Scene & Camera System
- **Người 2**: Models & Textures
- **Người 3**: Lighting & Shaders

## 📁 Cấu trúc dự án

```
detaidohoa/
├── core/           # Camera, Scene, Input
├── rendering/      # Shader, Light, Material
├── models/         # Mesh, Texture, Primitives
├── objects/        # Lang, CotCo, Ground, Tree
├── shaders/        # GLSL shader files
└── assets/         # Textures, models
```

Chi tiết xem: [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md)

## 🚀 Bắt đầu

1. **Đọc hướng dẫn**: [GETTING_STARTED.md](GETTING_STARTED.md)
2. **Xem task list**: [TASKS.md](TASKS.md)
3. **Tạo branch riêng** và bắt đầu code!

## 🔧 Build & Run

```bash
mkdir -p build && cd build
cmake ..
make
./DoAnApp
```

## 📚 Tài liệu tham khảo

- [LearnOpenGL](https://learnopengl.com/) - Tutorial chính
- [GLM Documentation](https://glm.g-truc.net/) - Math library

## ⚙️ Dependencies

- OpenGL 3.3+
- GLFW 3
- GLM
- GLAD
- GTK3 (Linux)
- stb_image (header-only)

## 📝 License

Educational project - Đồ án môn Đồ họa máy tính
