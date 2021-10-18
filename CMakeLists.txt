cmake_minimum_required(VERSION 3.20)
project(BakeEngine)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
include_directories(src/external/include)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw"
        GIT_TAG "3.3.4"
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(glfw)

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

add_executable(BakeEngine src/main.cpp src/external/gl.c src/external/vulkan.c src/Application/Application.cpp src/Application/Application.h
        src/Input/InputManager.cpp src/Input/InputManager.h src/Render/OpenGL/GLShader.cpp src/Render/OpenGL/GLShader.h
        src/Render/OpenGL/GLTexture.cpp src/Render/OpenGL/GLTexture.h src/FileParser/stb_image.cpp src/FileParser/stb_image.h
        src/Util/Math/Matrix4.cpp src/Util/Math/Matrix4.h src/Util/Math/Vector3.cpp src/Util/Math/Vector3.h src/Render/OpenGL/GLRenderer.cpp
        src/Render/OpenGL/GLRenderer.h src/Render/OpenGL/ElementBuffer.cpp src/Render/OpenGL/ElementBuffer.h
        src/Render/OpenGL/VertexArray.cpp src/Render/OpenGL/VertexArray.h src/Render/OpenGL/VertexBuffer.cpp
        src/Render/OpenGL/VertexBuffer.h src/Scene/Camera.cpp src/Scene/Camera.h src/Util/Math/Math.cpp src/Util/Math/Math.h src/Scene/Scene.cpp
        src/Scene/Scene.h src/Scene/Node.cpp src/Scene/Node.h src/Scene/Mesh.cpp src/Scene/Mesh.h src/Scene/Material.cpp src/Scene/Material.h
        src/Scene/Model.cpp src/Scene/Model.h src/FileParser/File.cpp src/FileParser/File.h src/FileParser/Model/Collada.cpp
        src/FileParser/Model/Collada.h src/FileParser/Model/fres/Fres.cpp src/FileParser/Model/fres/Fres.h
        src/Render/OpenGL/Framebuffer.cpp src/Render/OpenGL/Framebuffer.h src/Util/LogHelperBE.cpp src/Util/LogHelperBE.h
        src/FileParser/DataView.cpp src/FileParser/DataView.h src/UI/GuiElement.cpp src/UI/GuiElement.h src/Input/KeyBind.cpp
        src/Input/KeyBind.h src/UI/GuiButton.cpp src/UI/GuiButton.h src/FileParser/TrueType.cpp
        src/FileParser/TrueType.h src/Render/OpenGL/Renderbuffer.cpp src/Render/OpenGL/Renderbuffer.h)

include_directories(${GL_INCLUDE_DIR} ${VULKAN_INCLUDE_DIR})

target_link_libraries(BakeEngine PRIVATE glfw)
