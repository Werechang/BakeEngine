// Wir haben so vieles geschafft - wir schaffen das!
#include "Application.h"
#include "../Util/Math/Math.h"
#include "../Render/OpenGL/GLShader.h"
#include "../Render/OpenGL/GLTexture.h"
#include "../Render/OpenGL/VertexArray.h"
#include "../Render/OpenGL/ElementBuffer.h"
#include "../Render/OpenGL/Framebuffer.h"

void errorCallback(int error, const char *description) {
    LogHelperBE::pushName("GLFW");
    LogHelperBE::error((std::to_string(error) + ": " + description).c_str());
    LogHelperBE::popName();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    InputManager::updateKey(window, key, scancode, action, mods);
}

void mouseCallback(GLFWwindow *window, double xPos, double yPos) {
    InputManager::updateMouse(window, xPos, yPos);
}

void closeCallback(GLFWwindow* window) {

}

Application::Application(bool isOGL, int width, int height, const char *name) : isOGL(isOGL), width(width), height(height), name(name) {
    LogHelperBE::pushName("Application");
    if (isOGL) {
        LogHelperBE::info("Starting with OpenGL...");
    } else {
        LogHelperBE::info("Starting with Vulkan...");
        if (!glfwVulkanSupported()) {
            LogHelperBE::fatal("Vulkan is not supported on this device.");
            exit(-1);
        }
    }
    init();
}

void Application::init() {
    glfwInit();

    // Error function to print out errors
    glfwSetErrorCallback(errorCallback);

    // Specify OGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core for easier coding; newer devices only
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Anti-Aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Creates Application
    window = glfwCreateWindow(width, height, name, nullptr, nullptr);

    // exit when Application was not initialized properly
    if (window == nullptr) {
        LogHelperBE::error("Failed to create window context");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    // usually 0 but 1 is better for syncing reasons. On fast computers, buffers may be swapped
    // in the middle of a frame, leading to tearing.
    glfwSwapInterval(1);

    // Function for inputs
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetWindowCloseCallback(window, closeCallback);

    if (isOGL) {
        gladLoadGL();
    } else {

    }
}

void Application::start() {
    if (isOGL) {
        runGL();
    } else {
        runVk();
    }
    terminate();
}

void Application::terminate() {
    glfwDestroyWindow(window);
    glfwTerminate();
    running = false;
}

void Application::runGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);*/
    // 3f - position (x,y,z) | 3f - color (r,g,b) | 2f - texCoords (u,v) | 3f - normal (x,y,z) | each row a vertex
    float vertices[] = {
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,

            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    };
    int vertCount = sizeof(vertices)/4;
    // Position (2f), Texture coordinates (2f)
    float quadVerts[] = {
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };

    //renderer.addModel("../resources/standard.shader", vertices, sizeof(vertices)/sizeof(float), elementArray, sizeof(elementArray)/sizeof(unsigned int));

    // Vertex Array Object
    VertexArray vao;
    vao.bind();

    VertexBuffer vb(vertices, sizeof(vertices));
    vb.bind();

    // vertex position
    VertexAttributes attribs;
    // Position
    attribs.addAttribute<float>(3);
    // Color
    attribs.addAttribute<float>(3);
    // Texture coordinates
    attribs.addAttribute<float>(2);
    // Normals
    attribs.addAttribute<float>(3);

    vao.addVertexBuffer(vb, attribs);
    vao.unbind();
    vb.unbind();

    // Framebuffer
    VertexArray quadArray;
    quadArray.bind();
    VertexBuffer quadBuffer(quadVerts, sizeof(quadVerts));
    quadBuffer.bind();
    VertexAttributes quadAttribs;
    quadAttribs.addAttribute<float>(2);
    quadAttribs.addAttribute<float>(2);
    quadArray.addVertexBuffer(quadBuffer, quadAttribs);

    GLShader screenShader("../resources/framebuffer.shader");
    screenShader.bind();
    screenShader.uniform1i("screen", 0);

    Framebuffer fb(width, height, 4);

    Framebuffer ppb(width, height, 1, false);

    GLShader shader("../resources/standard.shader");
    shader.bind();

    GLTexture tex(GL_LINEAR, GL_REPEAT, "../resources/container2.png", TEXTURE_IMAGE);
    GLTexture texSpec(GL_LINEAR, GL_REPEAT, "../resources/container2_specular.png", TEXTURE_SPECULAR);
    shader.uniform1i("textureImage1", 0);
    shader.uniform1i("material.diffuse", 0);
    shader.uniform1i("material.specular", 1);

    shader.uniform3f("pointLights[0].position", 0.7f, 0.0f, 2.0f);
    shader.uniform3f("pointLights[0].ambient", 0.1f, 0.1f, 0.1f);
    shader.uniform3f("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    shader.uniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader.uniform1f("pointLights[0].constant", 1.0f);
    shader.uniform1f("pointLights[0].linear", 0.22f);
    shader.uniform1f("pointLights[0].quadratic", 0.2f);

    shader.uniform3f("pointLights[1].position", -2.7f, 1.0f, -1.0f);
    shader.uniform3f("pointLights[1].ambient", 0.1f, 0.1f, 0.1f);
    shader.uniform3f("pointLights[1].diffuse", 1.0f, 1.0f, 1.0f);
    shader.uniform3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader.uniform1f("pointLights[1].constant", 1.0f);
    shader.uniform1f("pointLights[1].linear", 0.7f);
    shader.uniform1f("pointLights[1].quadratic", 1.8f);

    shader.uniform3f("spotLights[0].position", 5.0f, 1.0f, 0.0f);
    shader.uniform3f("spotLights[0].direction", 1.0f, 0.0f, 0.0f);
    shader.uniform3f("spotLights[0].ambient", 0.01f, 0.01f, 0.01f);
    shader.uniform3f("spotLights[0].diffuse", 1.0f, 0.0f, 0.5f);
    shader.uniform3f("spotLights[0].specular", 1.0f, 0.0f, 0.5f);
    shader.uniform1f("spotLights[0].cutOff", Math::toRadians(12.5f));
    shader.uniform1f("spotLights[0].outerCutoff", Math::toRadians(15.0f));
    shader.uniform1f("spotLights[0].constant", 1.0f);
    shader.uniform1f("spotLights[0].linear", 0.14f);
    shader.uniform1f("spotLights[0].quadratic", 0.07f);


    shader.uniform3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.uniform3f("dirLight.ambient", 0.5f, 0.5f, 0.5f);
    shader.uniform3f("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader.uniform3f("dirLight.specular", 1.0f, 1.0f, 1.0f);

    Matrix4 model = Matrix4::identity();
    Matrix4 view = Matrix4::identity();
    Matrix4 projection = Matrix4::perspective(Math::toRadians(45), ((float)width)/((float)height), 0.1f, 1000.0f);

    shader.uniformMatrix4fv("projection", projection);
    shader.uniform1f("material.roughness", 32.0f);
    shader.unbind();

    // TODO: Frame skip, show FPS
    long long begin = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    unsigned int refreshRate = 1000000000/100;

    float deltaTime, lastFrame = 0;


    while (!glfwWindowShouldClose(window)) {

        // Time
        long long now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (now - begin >= refreshRate) {
            begin = now;

            auto currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            int wNew, hNew;
            glfwGetWindowSize(window, &wNew, &hNew);
            bool isResized = wNew != width || hNew != height;
            if (isResized) {
                width = wNew;
                height = hNew;
                glViewport(0, 0, width, height);
                ppb.resize(width, height);
                fb.resize(width, height);
                projection = Matrix4::perspective(Math::toRadians(45), ((float)width)/((float)height), 0.1f, 1000.0f);
                shader.bind();
                shader.uniformMatrix4fv("projection", projection);
                shader.unbind();
            }
            getInput(deltaTime);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            fb.bind();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            view = camera.getView();
            shader.bind();

            shader.uniform3f("cameraPos", camera.pos.x, camera.pos.y, camera.pos.z);
            shader.uniformMatrix4fv("view", view);
            shader.uniformMatrix4fv("model", model);

            vao.bind();
            tex.bind(0);
            texSpec.bind(1);
            glDrawArrays(GL_TRIANGLES, 0, vertCount);
            vao.unbind();

            fb.drawTo(&ppb);
            fb.unbind();
            screenShader.bind();
            quadArray.bind();
            glActiveTexture(GL_TEXTURE0);
            ppb.bindTexture();
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window);
            glfwPollEvents();

            long long frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - begin;
            if (refreshRate - frameTime > 0) {
                std::this_thread::sleep_for(std::chrono::nanoseconds(refreshRate - frameTime));
            }
        }
    }
    glfwSetWindowShouldClose(window, 0);
}

void Application::runVk() {

}

void Application::getInput(float deltaTime) {
    camera.speed = camera.globalSpeed * deltaTime;
    if (KeyArray[GLFW_KEY_W]) {
        camera.moveFront();
    }
    if (KeyArray[GLFW_KEY_S]) {
        camera.moveBack();
    }
    if (KeyArray[GLFW_KEY_A]) {
        camera.moveLeft();
    }
    if (KeyArray[GLFW_KEY_D]) {
        camera.moveRight();
    }

    float xOffset = (float)(mouseX - lastMouseX) * 0.1f;
    float yOffset = (float)(lastMouseY - mouseY) * 0.1f;
    camera.turn(xOffset, yOffset);
    lastMouseX = mouseX;
    lastMouseY = mouseY;
}
