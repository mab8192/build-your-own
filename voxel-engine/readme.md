# Voxel Engine

A simple voxel engine with destructible terrain written in C++ using raylib for rendering and CMake for dependency management.

## Prerequisites

- CMake 3.16 or higher
- Visual Studio 2022 (for Windows builds)
- Git (for downloading raylib if not available on system)
- raylib v5.5 or higher (will be automatically downloaded if not found)

## Building

### Windows

1. Run the build script:
   ```
   build.bat
   ```

2. The executable will be created at `build/bin/VoxelEngine.exe`

### Manual Build

1. Create a build directory:
   ```
   mkdir build
   cd build
   ```

2. Configure with CMake:
   ```
   cmake .. -G "Visual Studio 17 2022" -A x64
   ```

3. Build the project:
   ```
   cmake --build . --config Release
   ```

## Project Structure

- `src/` - Source files (.cpp and .h files)
- `CMakeLists.txt` - CMake configuration
- `build.bat` - Windows build script

## Features

- Simple spinning cube demo
- 3D rendering with raylib
- CMake-based build system
