# GLRenderer
Rasterizer that utilizes the OpenGL API developed by Khronos Group

# Features
- **As of now, there is nothing implemented (Except for a color interpolated triangle being rendered)**. This is just a placeholder for future development.

# Build Instructions

This project uses **CMake** to generate the build system. To build the project:

```bash
# Clone the repository and enter it
git clone https://github.com/Wobby24/GLRenderer.git
cd GLRenderer

# Create a separate build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .
