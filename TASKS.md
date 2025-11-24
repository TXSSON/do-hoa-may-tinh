# 👥 Task List - Dự án Lăng Bác OpenGL

## 👤 NGƯỜI 1: Core System (Scene & Camera)

### Tuần 1: Camera & Scene Setup
- [x] Khởi tạo GLFW window
- [x] Implement Camera class với WASD movement
- [x] Implement mouse look (xoay camera)
- [x] Implement scroll zoom
- [ ] Tạo class Scene để quản lý objects
- [ ] Test camera với 1 khối hộp đơn giản

### Tuần 2: Scene Management
- [ ] Implement addObject/removeObject
- [ ] Implement skybox (bầu trời)
- [ ] Tối ưu hóa render loop

### Tuần 3: Polish
- [ ] Thêm UI controls (pause, reset camera)
- [ ] Performance optimization
- [ ] Debug tools

---

## 👤 NGƯỜI 2: Models & Textures

### Tuần 1: Basic Geometry & Textures
- [ ] Tạo class Mesh (VAO/VBO/EBO)
- [ ] Tạo class Texture (load image với stb_image)
- [ ] Implement Primitives::createPlane()
- [ ] Implement Primitives::createBox()
- [ ] Test: Vẽ mặt phẳng có texture

### Tuần 2: Complex Models
- [ ] Tạo model Lăng Bác (bằng code hoặc load .obj)
- [ ] Tạo Cột cờ
- [ ] Tạo cây cối
- [ ] Texture mapping cho tất cả models

### Tuần 3: Details
- [ ] Thêm chi tiết nhỏ (đường đi, hàng rào...)
- [ ] Optimize mesh (LOD nếu cần)
- [ ] Final texturing

---

## 👤 NGƯỜI 3: Lighting & Shaders

### Tuần 1: Basic Lighting
- [x] Tạo basic shader (màu đơn giản)
- [ ] Viết Phong lighting shader
- [ ] Implement DirectionalLight (mặt trời)
- [ ] Test: Chiếu sáng khối hộp của Người 2

### Tuần 2: Advanced Lighting
- [ ] Implement PointLight (đèn đường)
- [ ] Shadow mapping (bóng đổ)
- [ ] Tối ưu shader performance

### Tuần 3: Effects
- [ ] Ambient occlusion (nếu có thời gian)
- [ ] Fog effect
- [ ] Final shader polish

---

## 🔗 Integration Milestones

### Milestone 1 (End of Week 1): "First Lit Object"
- [ ] Camera hoạt động
- [ ] Mặt phẳng có texture
- [ ] Ánh sáng mặt trời chiếu lên mặt phẳng
- [ ] Test: Có thể di chuyển camera xung quanh

### Milestone 2 (End of Week 2): "Scene with Lang"
- [ ] Mô hình Lăng đã hoàn thiện
- [ ] Có cột cờ và cây
- [ ] Shadow mapping hoạt động
- [ ] Skybox có bầu trời

### Milestone 3 (End of Week 3): "Final Product"
- [ ] Tất cả models hoàn thiện
- [ ] Lighting đẹp mắt
- [ ] Performance tối ưu (>30 FPS)
- [ ] Video demo
