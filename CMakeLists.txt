cmake_minimum_required(VERSION 4.2.1)

set(MY_GLOBAL_PROJECT_NAME DeltaEngine)

project(${MY_GLOBAL_PROJECT_NAME})

add_executable(${MY_GLOBAL_PROJECT_NAME} src/main.cpp)

set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build GLFW documentation")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build GLFW tests")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build GLFW examples")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target" )

add_subdirectory(ext/glfw)

target_link_libraries(${MY_GLOBAL_PROJECT_NAME} glfw)

find_package(OpenGL REQUIRED)