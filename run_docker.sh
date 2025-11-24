#!/bin/bash

# Build the Docker image
echo "Building Docker image..."
sudo docker build -t do-an-do-hoa .

# Allow X server connection
echo "Allowing X server connection..."
xhost +local:docker

# Run the container
echo "Running Docker container..."
sudo docker run -it --rm \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    --device /dev/dri:/dev/dri \
    do-an-do-hoa

# Revoke X server connection (optional, for security)
# xhost -local:docker
