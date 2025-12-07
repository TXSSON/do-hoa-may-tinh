# 🪟 Hướng Dẫn Chạy Trên Windows

Có 3 cách để chạy dự án này trên Windows, từ dễ đến khó:

1. **Docker** (Đã có sẵn setup, cần thêm X Server)
2. **WSL2** (Khuyên dùng cho Windows 10/11)
3. **Native Build** (Build trực tiếp .exe, cần setup môi trường)

---

## 🟢 Cách 1: Docker (Khuyên Dùng)

Do Windows không có X11 native, bạn cần cài một phần mềm X Server để hiển thị cửa sổ từ Docker.

### 1. Cài đặt VcXsrv
- Tải và cài đặt **VcXsrv** từ: [sourceforge.net/projects/vcxsrv/](https://sourceforge.net/projects/vcxsrv/)

### 2. Cấu hình VcXsrv (Làm mỗi lần chạy)
1. Mở **XLaunch**
2. Chọn **"Multiple windows"** -> Next
3. Chọn **"Start no client"** -> Next
4. **QUAN TRỌNG:** Tích vào **"Disable access control"** -> Next -> Finish

### 3. Chạy Docker
Mở PowerShell hoặc Command Prompt tại thư mục dự án và chạy:

```cmd
docker build -t do-an-do-hoa .

docker run -it --rm ^
  -e DISPLAY=host.docker.internal:0 ^
  do-an-do-hoa
```
*(Nếu dùng Git Bash, thay `^` bằng `\`)*

---

## 🟡 Cách 2: WSL2 (Windows Subsystem for Linux)

Nếu bạn đã cài WSL2 (Ubuntu), bạn có thể chạy dự án như trên Linux native.

### 1. Cài Dependencies trong WSL
Mở terminal Ubuntu (WSL) và chạy:
```bash
sudo apt-get update
sudo apt-get install cmake build-essential libglfw3-dev libglm-dev libgtk-3-dev libdecor-0-dev
```

### 2. Build và Run
```bash
./build.sh
cd build
./DoAnApp
```
*Lưu ý: Windows 11 hỗ trợ chạy GUI từ WSL2 trực tiếp (WSLg). Windows 10 có thể cần VcXsrv như Cách 1.*

---

## 🔴 Cách 3: Native Build (Visual Studio / MinGW)

Cách này sẽ tạo ra file `.exe` chạy trực tiếp trên Windows.

### 1. Cài đặt Công Cụ
- **CMake**: [cmake.org/download/](https://cmake.org/download/)
- **vcpkg** (Quản lý thư viện C++):
  ```cmd
  git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
  cd C:\vcpkg
  bootstrap-vcpkg.bat
  ```

### 2. Cài Thư Viện
```cmd
C:\vcpkg\vcpkg install glfw3:x64-windows glm:x64-windows
```

### 3. Cấu hình CMake
Dùng file `CMakeLists_Windows.txt` được cung cấp sẵn (đã bỏ các thư viện Linux-only như GTK3):

1. Đổi tên `CMakeLists.txt` gốc thành `CMakeLists_Linux.txt` (để backup)
2. Đổi tên `CMakeLists_Windows.txt` thành `CMakeLists.txt`

### 4. Build
```cmd
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

### 5. Run
Chạy file `.exe` trong thư mục `build/Release/`.

---

## 🎮 Điều khiển
- **WASD**: Di chuyển Camera
- **Chuột**: Xoay góc nhìn
- **T**: Dừng/Tiếp tục thời gian
- **U**: Kéo cờ lên
- **L**: Hạ cờ xuống
