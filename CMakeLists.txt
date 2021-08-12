cmake_minimum_required(VERSION 3.20)
project(BakeEngine)

set(CMAKE_CXX_STANDARD 23)
set(GL_INCLUDE_DIR external/Libraries/GL/include)
set(GL_LIB_DIR external/Libraries/GL/lib)
set(GLFW_USE_STATIC_LIBS ON)

set(VULKAN_INCLUDE_DIR external/Libraries/Vulkan/include)
set(VULKAN_LIB_DIR external/Libraries/Vulkan/lib)

add_executable(BakeEngine main.cpp external/glad.c Application/Application.cpp Application/Application.h Input/InputManager.cpp Input/InputManager.h Render/OpenGL/GLShader.cpp Render/OpenGL/GLShader.h Render/OpenGL/GLTexture.cpp Render/OpenGL/GLTexture.h FileParser/stb_image.cpp FileParser/stb_image.h Util/Math/Matrix4.cpp Util/Math/Matrix4.h Util/Math/Vector3.cpp Util/Math/Vector3.h Util/Math/Angle.cpp Util/Math/Angle.h)

include_directories(${GL_INCLUDE_DIR} ${VULKAN_INCLUDE_DIR})

add_library(glfw STATIC IMPORTED)
add_library(vulkan-1 STATIC IMPORTED)

set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/${GL_LIB_DIR}/glfw3.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/${GL_INCLUDE_DIR}")
set_target_properties(vulkan-1 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/${VULKAN_LIB_DIR}/vulkan-1.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/${VULKAN_INCLUDE_DIR}")

target_link_libraries(BakeEngine glfw)
target_link_libraries(BakeEngine vulkan-1)