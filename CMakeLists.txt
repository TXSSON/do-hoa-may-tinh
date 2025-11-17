cmake_minimum_required(VERSION 3.10)
project(DoAnDoHoa)

# Đặt chuẩn C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Tìm các thư viện đã cài bằng Homebrew
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Thêm thư mục "glad/include" để tìm file "glad.h"
include_directories(glad/include)

# Tạo file thực thi (executable)
add_executable(
    DoAnApp     # Tên chương trình của bạn
    main.cpp
    glad/src/glad.c # Phải build cả file .c của GLAD
)

# Link các thư viện cần thiết
target_link_libraries(
    DoAnApp
    glfw        # Thư viện GLFW
    ${OPENGL_LIBRARIES} # Thư viện OpenGL của hệ thống
)