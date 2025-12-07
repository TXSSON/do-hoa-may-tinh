# 🚀 Hướng Dẫn Build & Run Đơn Giản

## Quick Start - Chỉ Một Câu Lệnh!

### **Linux / macOS**
```bash
./build.sh
```

### **Windows**
```bash
build.bat
```

Hoặc trên Git Bash/MSYS2:
```bash
./build.sh
```

---

## ✅ Script Tự Động Làm Gì?

1. **Tự nhận biết hệ điều hành** (Linux/Mac/Windows)
2. **Kiểm tra dependencies** (CMake, GLFW, GLM)
3. **Build project** với CMake
4. **Chạy luôn** ứng dụng

---

## 📦 Cài Dependencies (Chỉ cần làm 1 lần)

### **Linux (Ubuntu/Debian)**
```bash
sudo apt-get install cmake libglfw3-dev libglm-dev
```

### **macOS (Homebrew)**
```bash
brew install cmake glfw glm
```

### **Windows (vcpkg - Recommended)**
```bash
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat

# Install dependencies
.\vcpkg install glfw3 glm
```

---

## 🐳 Docker (Tất Cả Platform)

Nếu không muốn cài dependencies:

```bash
./run_docker.sh
```

> **Windows/macOS:** Cần cài X server (VcXsrv/XQuartz) để hiển thị GUI

---

## ❓ Troubleshooting

### Lỗi: "GLFW not found"
- **Linux:** `sudo apt install libglfw3-dev`
- **Mac:** `brew install glfw`
- **Windows:** Cài vcpkg và chạy `vcpkg install glfw3`

### Lỗi: "Permission denied" (Linux/Mac)
```bash
chmod +x build.sh
```

### Lỗi build trên Windows
Đảm bảo đã cài:
- Visual Studio Build Tools hoặc MinGW
- vcpkg với dependencies đúng

---

## 📝 Notes

- Script tự động phát hiện số CPU cores để build nhanh hơn
- Build output ở folder `build/`
- Không cần config gì thêm, chỉ cần chạy script!
