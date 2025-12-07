# 🍎 Hướng Dẫn Chạy Trên macOS

## 🚀 Bước 1: Cài Đặt Dependencies

### **Cài Homebrew (Nếu chưa có)**
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### **Cài OpenGL Dependencies**
```bash
brew install cmake glfw glm
```

---

## 📦 Bước 2: Build Project

```bash
# Clone hoặc copy project về Mac
cd /path/to/detaidohoa

# Tạo thư mục build
mkdir build
cd build

# Configure với CMake
cmake ..

# Compile
make -j4

# Chạy app
./DoAnApp
```

---

## ⚡ Quick Start (Dùng Script)

Sau khi cài dependencies:

```bash
# Từ thư mục gốc project
./build.sh
cd build
./DoAnApp
```

---

## ⚠️ Troubleshooting

### **Lỗi: "cmake: command not found"**
```bash
brew install cmake
```

### **Lỗi: "Package glfw3 not found"**
```bash
brew install glfw
```

### **Lỗi: GTK3/libdecor not found**
Đây là dependencies cho Linux. Trên macOS sửa `CMakeLists.txt`:

```cmake
# Tìm dòng này (khoảng line 12-17)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Sửa thành:
if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
endif()
```

Và dòng link libraries (line 50-57):

```cmake
target_link_libraries(
    DoAnApp
    glfw
    ${OPENGL_LIBRARIES}
)

# Thêm sau đó:
if(UNIX AND NOT APPLE)
    target_link_libraries(DoAnApp ${GTK3_LIBRARIES} decor-0)
endif()
```

---

## 🎮 Controls

- **WASD** - Di chuyển camera
- **Mouse** - Xoay camera
- **Scroll** - Zoom in/out
- **T** - Pause time
- **U** - Raise flag
- **L** - Lower flag
- **ESC** - Thoát

---

## 📝 Notes

- macOS dùng **Cocoa** thay vì X11, GLFW tự động detect
- Không cần GTK3/libdecor trên macOS
- OpenGL có sẵn trên macOS (Metal backend)

---

## 🐳 Alternative: Dùng Docker

Nếu muốn dùng Docker (phức tạp hơn):

```bash
# Cài XQuartz
brew install --cask xquartz

# Khởi động XQuartz và enable network
xhost + localhost

# Build và run
docker build -t do-an-do-hoa .
docker run -it --rm \
    -e DISPLAY=host.docker.internal:0 \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    do-an-do-hoa
```

**Khuyến nghị:** Build native dễ hơn nhiều!

---

## ✅ Summary

**Cách Đơn Giản Nhất:**
```bash
brew install cmake glfw glm
./build.sh
cd build && ./DoAnApp
```

Xong! 🎉
