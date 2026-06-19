cmake_minimum_required(VERSION 4.2.1)

project(Glad)

add_library(Glad STATIC src/glad.c)
target_include_directories(Glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)