// Wir haben so vieles geschafft - wir schaffen das!
#include "Application.h"
#include "../Render/OpenGL/Framebuffer.h"
#include "../FileParser/stb_image.h"

/*#define glCheckError() glCheckError_(__FILE__, __LINE__)

GLenum glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        LogHelperBE::pushName("OpenGL");
        LogHelperBE::error(error + " | " + file + " (" + std::to_string(line) + ")");
        LogHelperBE::popName();
    }
    return errorCode;
}*/

void errorCallback(int error, const char *description) {
    LogHelperBE::pushName("GLFW");
    LogHelperBE::error(std::to_string(error) + ": " + description);
    LogHelperBE::popName();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    inputManagerPtr->updateKey(window, key, scancode, action, mods);
}

void mouseCallback(GLFWwindow *window, double xPos, double yPos) {
    inputManagerPtr->updateMouse(window, xPos, yPos);
}

void closeCallback(GLFWwindow* window) {
    running = false;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    inputManagerPtr->updateMouseButton(window, button, action, mods);
}

void focusCallback(GLFWwindow* window, int focused) {

}

Application::Application(bool isOGL, int width, int height, const std::string& name) : isOGL(isOGL), width(width), height(height), name(name) {
    LogHelperBE::pushName("Application");
    if (isOGL) {
        LogHelperBE::info("Starting with OpenGL...");
    } else {
        LogHelperBE::info("Starting with Vulkan...");
        if (!glfwVulkanSupported()) {
            LogHelperBE::fatal("Vulkan is not supported on this device.");
        }
    }
    init();
}

void Application::init() {
    glfwInit();

    // Specify OGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    // Core for easier coding; newer devices only
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Anti-Aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Creates Application
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

    // exit when Application was not initialized properly
    if (window == nullptr) {
        LogHelperBE::error("Failed to create window context");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowTitle(window, "BakeEngine");

    // usually 0 but 1 is better for syncing reasons. On fast computers, buffers may be swapped
    // in the middle of a frame, leading to tearing. It also caps the refresh rate
    glfwSwapInterval(1);

    // Function for callbacks
    inputManagerPtr = new InputManager();
    glfwSetErrorCallback(errorCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetWindowCloseCallback(window, closeCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetWindowFocusCallback(window, focusCallback);

    if (isOGL) {
        int version = gladLoadGL(glfwGetProcAddress);
        if (version == 0) {
            LogHelperBE::fatal("Failed to load OpenGL");
        }
        LogHelperBE::info("Loaded OpenGL successfully");
        if (!GLAD_GL_EXT_texture_filter_anisotropic)
            LogHelperBE::error("Anisotropic filtering is not supported!");
    } else {
        // Load vulkan
    }
    glfwSetWindowShouldClose(window, 0);
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
    delete inputManagerPtr;
}

void Application::runGL() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
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
    int vertCount = sizeof(vertices)/11;
    // Position (2f), Texture coordinates (2f)
    float quadVerts[] = {
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };
    GLRenderer glRenderer = GLRenderer("shaders/gui.shader", width, height);
    GuiElement gui1(1400.0f, 50.0f, 150.0f, 150.0f, GUI_AXIS_Y, GUI_TOP_RIGHT, "textures/gui_placeholder.png");
    GuiElement gui2(50.0f, 50.0f, 150.0f, 150.0f, GUI_AXIS_Y, GUI_TOP_LEFT, "textures/gui_placeholder.png");

    inputManagerPtr->addKeyBind([this] { camera.moveFront(); }, GLFW_KEY_W, 0, INPUT_ON_ACTIVE);
    inputManagerPtr->addKeyBind([this] { camera.moveLeft(); }, GLFW_KEY_A, 0, INPUT_ON_ACTIVE);
    inputManagerPtr->addKeyBind([this] { camera.moveBack(); }, GLFW_KEY_S, 0, INPUT_ON_ACTIVE);
    inputManagerPtr->addKeyBind([this] { camera.moveRight(); }, GLFW_KEY_D, 0, INPUT_ON_ACTIVE);
    inputManagerPtr->addKeyBind([this] { camera.moveUp(); }, GLFW_KEY_Q, 0, INPUT_ON_ACTIVE);
    inputManagerPtr->addKeyBind([this] { camera.moveDown(); }, GLFW_KEY_E, 0, INPUT_ON_ACTIVE);
    inputManagerPtr->addKeyBind([this] { return glfwSetWindowShouldClose(window, true); }, GLFW_KEY_Q, GLFW_MOD_CONTROL, INPUT_ON_PRESS);
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
    quadArray.unbind();
    quadBuffer.unbind();

    GLShader screenShader("shaders/framebuffer.shader");
    screenShader.bind();
    screenShader.uniform1i("screen", 0);

    GLShader screenDBShader("shaders/fbDepth.shader");
    screenDBShader.bind();
    screenDBShader.uniform1i("screen", 0);

    Framebuffer fb(width, height, 4);

    Framebuffer viewport(width, height, 1, false);

    GLShader shader("shaders/standard.shader");
    shader.bind();

    GLTexture tex(GL_LINEAR, GL_REPEAT, "textures/container2.png", TEXTURE_IMAGE, false, 8.0f, true);
    GLTexture texSpec(GL_LINEAR, GL_REPEAT, "textures/container2_specular.png", TEXTURE_SPECULAR, false, 8.0f, true);
    shader.uniform1i("textureImage1", 0);
    shader.uniform1i("material.diffuse", 0);
    shader.uniform1i("material.specular", 1);

    shader.uniform3f("pointLights[0].position", 0.7f, 0.0f, 2.0f);
    shader.uniform3f("pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
    shader.uniform3f("pointLights[0].diffuse", 1.f, 1.0f, 1.0f);
    shader.uniform3f("pointLights[0].specular", 0.5f, 0.5f, 0.5f);

    shader.uniform3f("pointLights[1].position", -2.7f, 1.0f, -1.0f);
    shader.uniform3f("pointLights[1].ambient", 0.0f, 0.0f, 0.0f);
    shader.uniform3f("pointLights[1].diffuse", 0.5f, 0.5f, 0.5f);
    shader.uniform3f("pointLights[1].specular", 0.5f, 0.5f, 0.5f);

    shader.uniform3f("spotLights[0].position", 5.0f, 1.0f, 0.0f);
    shader.uniform3f("spotLights[0].direction", 1.0f, 0.0f, 0.0f);
    shader.uniform3f("spotLights[0].ambient", 0.0f, 0.0f, 0.0f);
    shader.uniform3f("spotLights[0].diffuse", 0.5f, 0.0f, 0.25f);
    shader.uniform3f("spotLights[0].specular", 0.5f, 0.0f, 0.25f);
    shader.uniform1f("spotLights[0].cutOff", toRadians(12.5f));
    shader.uniform1f("spotLights[0].outerCutoff", toRadians(15.0f));

    shader.uniform3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.uniform3f("dirLight.ambient", 0.2f, 0.2f, 0.2f);
    shader.uniform3f("dirLight.diffuse", 2.0f, 2.0f, 2.0f);
    shader.uniform3f("dirLight.specular", 0.8f, 0.8f, 0.8f);

    Matrix4 model = Matrix4::identity();
    Matrix4 view = Matrix4::identity();
    Matrix4 projection = Matrix4::perspective(toRadians(45), ((float)width)/((float)height), 0.1f, 1000.0f);

    shader.uniformMatrix4fv("projection", projection);
    shader.uniform1f("material.roughness", 256.0f);
    shader.unbind();

    // TODO Frame skip
    double last = glfwGetTime();

    double deltaTime;

    while (!glfwWindowShouldClose(window) && running) {

        // Time
        double thisFrame = glfwGetTime();
        deltaTime = thisFrame - last;
        last = thisFrame;

        int wNew, hNew;
        glfwGetFramebufferSize(window, &wNew, &hNew);
        bool isResized = wNew != width || hNew != height;
        if (isResized) {
            width = wNew;
            height = hNew;
            glViewport(0, 0, width, height);
            viewport.resize(width, height);
            fb.resize(width, height);
            projection = Matrix4::perspective(toRadians(45), ((float)width)/((float)height), 0.1f, 1000.0f);
            shader.bind();
            shader.uniformMatrix4fv("projection", projection);
            shader.unbind();
            glRenderer.onResize(width, height);
        }
        glfwPollEvents();
        // Same speed for different framerate
        // TODO Lower camera speed if two keys are pressed (i.e. WA)
        camera.speed = camera.globalSpeed * deltaTime;
        inputManagerPtr->updateInput(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        fb.bind();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        view = camera.getView();
        shader.bind();

        shader.uniform3f("cameraPos", camera.getPos().x, camera.getPos().y, camera.getPos().z);
        shader.uniformMatrix4fv("view", view);
        shader.uniformMatrix4fv("model", model);

        vao.bind();
        tex.bind(0);
        texSpec.bind(1);
        glDrawArrays(GL_TRIANGLES, 0, vertCount);
        vao.unbind();

        fb.drawTo(viewport);
        fb.unbind();
        glDisable(GL_DEPTH_TEST);
        screenShader.bind();
        quadArray.bind();
        glActiveTexture(GL_TEXTURE0);
        viewport.bindTexture();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //glRenderer.draw();

        glfwSwapBuffers(window);
    }
}

void Application::runVk() {

}
