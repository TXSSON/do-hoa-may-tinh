@echo off
REM Windows Build Script for OpenGL Project

echo === Cross-Platform OpenGL Build Script (Windows) ===
echo.

REM Check for CMake
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake not found!
    echo Install from: https://cmake.org/download/
    exit /b 1
)

REM Check for vcpkg (optional but recommended)
if exist "C:\vcpkg\scripts\buildsystems\vcpkg.cmake" (
    echo Found vcpkg at C:\vcpkg
    set VCPKG_TOOLCHAIN=-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
) else if exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
    echo Found vcpkg at %VCPKG_ROOT%
    set VCPKG_TOOLCHAIN=-DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake
) else (
    echo WARNING: vcpkg not found. Dependencies must be installed manually.
    echo Install vcpkg: https://vcpkg.io/en/getting-started.html
    echo Then run: vcpkg install glfw3 glm
    set VCPKG_TOOLCHAIN=
)

echo.
echo === Building Project ===

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake
echo Configuring with CMake...
cmake .. %VCPKG_TOOLCHAIN% || (
    echo CMake configuration failed!
    cd ..
    exit /b 1
)

REM Build
echo Compiling...
cmake --build . --config Release || (
    echo Build failed!
    cd ..
    exit /b 1
)

cd ..

echo.
echo === Build Successful! ===
echo.

REM Run the application
if exist "build\Release\DoAnApp.exe" (
    echo Running application...
    build\Release\DoAnApp.exe
) else if exist "build\DoAnApp.exe" (
    echo Running application...
    build\DoAnApp.exe
) else (
    echo ERROR: Executable not found!
    exit /b 1
)
