cmake_minimum_required(VERSION 3.20)
project(BakeEngine)

set(CMAKE_CXX_STANDARD 23)
set(GL_INCLUDE_DIR external/Libraries/include)
set(GL_LIB_DIR external/Libraries/lib)
set(GLFW_USE_STATIC_LIBS ON)

add_executable(BakeEngine main.cpp external/glad.c window/Application.cpp window/Application.h window/OpenGLApp.cpp window/OpenGLApp.h window/AbstractApplication.h)

include_directories(${GL_INCLUDE_DIR})
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/${GL_LIB_DIR}/glfw3.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/${GL_INCLUDE_DIR}")
target_link_libraries(BakeEngine glfw)