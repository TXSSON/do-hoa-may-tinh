#!/bin/bash
# Universal Build Script - Auto-detect platform and build

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Cross-Platform OpenGL Build Script ===${NC}"

# Detect OS
OS="$(uname -s)"
case "${OS}" in
    Linux*)     PLATFORM=Linux;;
    Darwin*)    PLATFORM=Mac;;
    CYGWIN*|MINGW*|MSYS*)    PLATFORM=Windows;;
    *)          PLATFORM="UNKNOWN:${OS}"
esac

echo -e "${YELLOW}Detected Platform: ${PLATFORM}${NC}"

# Check dependencies
check_dependencies() {
    echo -e "${YELLOW}Checking dependencies...${NC}"
    
    if ! command -v cmake &> /dev/null; then
        echo -e "${RED}ERROR: CMake not found!${NC}"
        echo "Install: sudo apt install cmake (Linux) or brew install cmake (Mac)"
        exit 1
    fi
    
    if ! command -v make &> /dev/null; then
        echo -e "${RED}ERROR: Make not found!${NC}"
        exit 1
    fi
    
    echo -e "${GREEN}✓ Build tools found${NC}"
}

# Platform-specific setup
setup_platform() {
    case "${PLATFORM}" in
        Linux)
            echo -e "${YELLOW}Linux detected - checking GLFW/GLM...${NC}"
            # Check if dev packages are installed
            if ! pkg-config --exists glfw3 2>/dev/null; then
                echo -e "${YELLOW}GLFW not found. Install with:${NC}"
                echo "  sudo apt-get install libglfw3-dev libglm-dev"
                echo -e "${YELLOW}Continuing anyway (may fail)...${NC}"
            fi
            ;;
        Mac)
            echo -e "${YELLOW}macOS detected - checking Homebrew packages...${NC}"
            if ! brew list glfw &> /dev/null; then
                echo -e "${YELLOW}GLFW not found. Install with:${NC}"
                echo "  brew install glfw glm"
                echo -e "${YELLOW}Continuing anyway (may fail)...${NC}"
            fi
            ;;
        Windows)
            echo -e "${YELLOW}Windows detected (Git Bash/MinGW/MSYS)${NC}"
            echo -e "${YELLOW}Make sure vcpkg dependencies are installed!${NC}"
            ;;
    esac
}

# Build
build_project() {
    echo -e "${GREEN}=== Building Project ===${NC}"
    
    # Create build directory
    mkdir -p build
    cd build
    
    # Configure
    echo -e "${YELLOW}Configuring with CMake...${NC}"
    cmake .. || {
        echo -e "${RED}CMake configuration failed!${NC}"
        exit 1
    }
    
    # Build
    echo -e "${YELLOW}Compiling...${NC}"
    make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4) || {
        echo -e "${RED}Build failed!${NC}"
        exit 1
    }
    
    cd ..
    echo -e "${GREEN}✓ Build successful!${NC}"
}

# Instructions to run
run_project() {
    echo -e "${GREEN}=== Build Successful! ===${NC}"
    echo ""
    echo -e "${YELLOW}To run the application:${NC}"
    echo "  cd build"
    echo "  ./DoAnApp"
    echo ""
    echo -e "${GREEN}Or run directly:${NC}"
    echo "  build/DoAnApp"
}

# Main execution
main() {
    check_dependencies
    setup_platform
    build_project
    run_project
}

main
