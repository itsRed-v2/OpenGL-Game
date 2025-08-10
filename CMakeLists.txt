cmake_minimum_required(VERSION 3.10)

project(Voxels VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Make an executable named Voxels from source files
add_executable(Voxels
            src/main.cpp
            src/shader.cpp
            src/fpsCounter.cpp
            src/texture2D.cpp
            src/camera.cpp
            src/chunk.cpp
            src/world.cpp
            src/math/vectors.cpp
            )

# Add GLAD
add_subdirectory(lib/glad)
target_link_libraries(Voxels PUBLIC glad)

# add glfw and link it
add_subdirectory(lib/glfw-3.4)
target_link_libraries(Voxels PUBLIC glfw)

# Add include directories
target_include_directories(Voxels PUBLIC
                           "${PROJECT_SOURCE_DIR}/lib/glad/include"
                           "${PROJECT_SOURCE_DIR}/lib/header-only"
                           "${PROJECT_SOURCE_DIR}/src"
                           )

# Copy assets to binary folder
add_custom_target(copy_assets
    # Empty the build assets folder
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/assets/*
    # Copy source assets to build assets folder
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)
add_dependencies(Voxels copy_assets) # Add this target as a dependency to the executable so it gets run every time