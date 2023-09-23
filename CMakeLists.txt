cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# setting the project name to Terrain
project(Terrain)

# Adding the subdirectory that contains GLFW library
add_subdirectory("C:/Users/chand/Documents/CG_Procedural_terrain/lib/glfw-3.3.8")

# find the OpenGL library
find_package(OpenGL REQUIRED)

# Include all the necessary libraries from the lib folder in source directory
include_directories(${OPENGL_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.8/include)

# adding all the source files to the executable
add_executable(Terrain TerrainShader.cpp main.cpp Camera.cpp terrain.cpp water.cpp LightPos.cpp lib/glad/glad.c)

#link OpenGL libraries to the executable
target_link_libraries(Terrain PUBLIC ${OPENGL_LIBRARIES} glfw)