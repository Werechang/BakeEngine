cmake_minimum_required(VERSION 3.20)
project(BakeEngine)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
include_directories(external/include)

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

add_executable(BakeEngine main.cpp external/gl.c external/vulkan.c Application/Application.cpp Application/Application.h Input/InputManager.cpp Input/InputManager.h Render/OpenGL/GLShader.cpp Render/OpenGL/GLShader.h Render/OpenGL/GLTexture.cpp Render/OpenGL/GLTexture.h FileParser/stb_image.cpp FileParser/stb_image.h Util/Math/Matrix4.cpp Util/Math/Matrix4.h Util/Math/Vector3.cpp Util/Math/Vector3.h Render/OpenGL/GLRenderer.cpp Render/OpenGL/GLRenderer.h Render/OpenGL/ElementBuffer.cpp Render/OpenGL/ElementBuffer.h Render/OpenGL/VertexArray.cpp Render/OpenGL/VertexArray.h Render/OpenGL/VertexBuffer.cpp Render/OpenGL/VertexBuffer.h Scene/Camera.cpp Scene/Camera.h Util/Math/Math.cpp Util/Math/Math.h Scene/Scene.cpp Scene/Scene.h Scene/Node.cpp Scene/Node.h Scene/Mesh.cpp Scene/Mesh.h Scene/Material.cpp Scene/Material.h Scene/Model.cpp Scene/Model.h FileParser/File.cpp FileParser/File.h FileParser/Model/Collada.cpp FileParser/Model/Collada.h FileParser/Model/fres/Fres.cpp FileParser/Model/fres/Fres.h Render/OpenGL/Framebuffer.cpp Render/OpenGL/Framebuffer.h Util/LogHelperBE.cpp Util/LogHelperBE.h FileParser/DataView.cpp FileParser/DataView.h UI/GuiElement.cpp UI/GuiElement.h Input/KeyBind.cpp Input/KeyBind.h Input/InputCallable.h UI/GuiButton.cpp UI/GuiButton.h FileParser/TrueType.cpp FileParser/TrueType.h)

include_directories(${GL_INCLUDE_DIR} ${VULKAN_INCLUDE_DIR})

target_link_libraries(BakeEngine PRIVATE glfw)
