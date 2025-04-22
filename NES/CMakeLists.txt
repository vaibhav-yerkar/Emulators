cmake_minimum_required(VERSION 3.10)

# Set C++17 standard 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set debug type 
set(CMAKE_BUILD_TYPE Debug)

project(nes VERSION 1.0)

find_package(SDL2 REQUIRED)
find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(RESOURCE_PATH "${CMAKE_SOURCE_DIR}")
add_compile_definitions(RESOURCE_PATH="${RESOURCE_PATH}")


file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
add_executable(nes ${SOURCE_FILES})

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(nes PRIVATE ${SDL2_LIBRARIES} ${PNG_LIBRARIES} OpenGL::GL GLUT::GLUT "-framework OpenGL" "-framework GLUT")
