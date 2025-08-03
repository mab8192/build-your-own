@echo off
echo Building Voxel Engine...

REM Create build directory if it doesn't exist
if not exist "build" mkdir build
cd build

REM Configure with CMake
echo Configuring with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64

REM Build the project
echo Building project...
cmake --build . --config Release

echo Build complete!
echo Executable is located in: build\bin\VoxelEngine.exe
pause 