cmake_minimum_required(VERSION 4.2.1)

set(MY_GLOBAL_PROJECT_NAME DeltaEngine)

project(${MY_GLOBAL_PROJECT_NAME})

add_executable(${MY_GLOBAL_PROJECT_NAME} 
src/main.cpp
#ext/glad/src/glad.c
)

set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build GLFW documentation")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build GLFW tests")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build GLFW examples")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target" )

add_subdirectory(ext/glfw)

add_subdirectory(ext/glad)
target_link_libraries(${MY_GLOBAL_PROJECT_NAME} PRIVATE 
Glad 
glfw
)

#find_package(glad REQUIRED)
find_package(OpenGL REQUIRED)