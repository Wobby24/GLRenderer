# GLRenderer
Rasterizer that utilizes the OpenGL API developed by Khronos Group

# Features
- **As of now, there is not much implemented**. This is just a placeholder for future development.

# Requirements:
- System capable of using OpenGL 3.3
- CPU and RAM haven't been decided yet, but the program doesn't need much processing power to get good results (as of now)
- CMake (setup script handles this). Version 3.16 is required
- Modern C++ compiler with C++ 20 support

# Build Instructions

1. **If on Linux, you must run setup.sh in order to have the required packages needed to run the project** 
1a. To make the script executable, run:
```bash  chmod +x setup.sh
```

2. This repository uses **CMake** for building. Below are the instructions to build the repository:

```bash
# Clone the repository and enter it. This step isn't required, you can download it, but this is better.
git clone https://github.com/Wobby24/GLRenderer.git
cd GLRenderer

# Create a separate build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .
```