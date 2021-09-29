cmake_minimum_required(VERSION 3.20)
project(BakeEngine)

set(CMAKE_CXX_STANDARD 23)
set(GL_INCLUDE_DIR external/Libraries/GL/include)
set(GL_LIB_DIR external/Libraries/GL/lib)
set(GLFW_USE_STATIC_LIBS ON)

set(VULKAN_INCLUDE_DIR external/Libraries/Vulkan/include)
set(VULKAN_LIB_DIR external/Libraries/Vulkan/lib)
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    set(data_SHADOW resources)
    foreach(item IN LISTS data_SHADOW)
        message(STATUS ${item})
        add_custom_command(
                OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
                COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${item}" "${CMAKE_CURRENT_BINARY_DIR}/${item}"
                DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
        )
    endforeach()
endif()

add_custom_target(data-target ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_executable(BakeEngine main.cpp external/glad.c Application/Application.cpp Application/Application.h Input/InputManager.cpp Input/InputManager.h Render/OpenGL/GLShader.cpp Render/OpenGL/GLShader.h Render/OpenGL/GLTexture.cpp Render/OpenGL/GLTexture.h FileParser/stb_image.cpp FileParser/stb_image.h Util/Math/Matrix4.cpp Util/Math/Matrix4.h Util/Math/Vector3.cpp Util/Math/Vector3.h Render/OpenGL/GLRenderer.cpp Render/OpenGL/GLRenderer.h Render/OpenGL/ElementBuffer.cpp Render/OpenGL/ElementBuffer.h Render/OpenGL/VertexArray.cpp Render/OpenGL/VertexArray.h Render/OpenGL/VertexBuffer.cpp Render/OpenGL/VertexBuffer.h Scene/Camera.cpp Scene/Camera.h Util/Math/Math.cpp Util/Math/Math.h Scene/Scene.cpp Scene/Scene.h Scene/Node.cpp Scene/Node.h Scene/Mesh.cpp Scene/Mesh.h Scene/Material.cpp Scene/Material.h Scene/Model.cpp Scene/Model.h FileParser/File.cpp FileParser/File.h FileParser/Model/Collada.cpp FileParser/Model/Collada.h FileParser/Model/fres/Fres.cpp FileParser/Model/fres/Fres.h Render/OpenGL/Framebuffer.cpp Render/OpenGL/Framebuffer.h Util/LogHelperBE.cpp Util/LogHelperBE.h FileParser/DataView.cpp FileParser/DataView.h UI/GuiElement.cpp UI/GuiElement.h Input/InputNode.cpp Input/InputNode.h Input/InputCallable.h)

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
