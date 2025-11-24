FROM ubuntu:22.04

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies and libraries
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libglfw3-dev \
    libglm-dev \
    libgtk-3-dev \
    libdecor-0-dev \
    libgl1-mesa-dev \
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Create build directory
RUN mkdir -p build

# Build the project
WORKDIR /app/build
RUN cmake .. && make -j$(nproc)

# Set the entrypoint to run the application
CMD ["./DoAnApp"]
