cmake_minimum_required(VERSION 4.2.1)

set(MY_GLOBAL_PROJECT_NAME DeltaEngine)

project(${MY_GLOBAL_PROJECT_NAME})

set(CMAKE_C_STANDARD       99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS      OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(CMAKE_C_FLAGS_DEBUG   "-O0 -g" CACHE STRING "C Debug flags"   FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g" CACHE STRING "C++ Debug flags" FORCE)

set(CMAKE_C_FLAGS_RELEASE   "-O2 -DNDEBUG -s" CACHE STRING "C Release flags"   FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -s" CACHE STRING "C++ Release flags" FORCE)


add_executable(${MY_GLOBAL_PROJECT_NAME} 
    src/main.cpp
    #ext/glad/src/glad.c
    src/render/objects/shaders/shader_program.cpp
    src/render/resource_manager.cpp
    src/log/logger.cpp
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


set_target_properties(${MY_GLOBAL_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
)
add_custom_command(TARGET ${MY_GLOBAL_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/data $<TARGET_FILE_DIR:${MY_GLOBAL_PROJECT_NAME}>/data
)