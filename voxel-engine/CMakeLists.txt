cmake_minimum_required(VERSION 3.16)
project(VoxelEngine VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find source files using glob patterns
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

# Create executable
add_executable(VoxelEngine ${SOURCES} ${HEADERS})

# Add raylib as a dependency
# Try to find raylib on the system first
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(RAYLIB QUIET raylib>=5.5)
endif()

# If raylib is not found on the system, fetch it from GitHub
if(NOT RAYLIB_FOUND)
    message(STATUS "raylib not found on system, fetching from GitHub...")
    
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
    )
    FetchContent_MakeAvailable(raylib)
    
    # Set raylib variables for consistency
    set(RAYLIB_LIBRARIES raylib)
    set(RAYLIB_INCLUDE_DIRS ${raylib_SOURCE_DIR}/src)
    set(RAYLIB_FOUND TRUE)
    
    message(STATUS "raylib fetched and built successfully")
else()
    message(STATUS "raylib found on system")
endif()

# Link raylib
target_link_libraries(VoxelEngine ${RAYLIB_LIBRARIES})
target_include_directories(VoxelEngine PRIVATE ${RAYLIB_INCLUDE_DIRS})
if(RAYLIB_CFLAGS_OTHER)
    target_compile_options(VoxelEngine PRIVATE ${RAYLIB_CFLAGS_OTHER})
endif()

# Set compiler flags
if(MSVC)
    target_compile_options(VoxelEngine PRIVATE /W4)
else()
    target_compile_options(VoxelEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Print information about the build
message(STATUS "Building VoxelEngine v${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source files: ${SOURCES}")
message(STATUS "Header files: ${HEADERS}") 