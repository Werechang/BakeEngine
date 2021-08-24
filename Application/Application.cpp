// Wir haben so vieles geschafft - wir schaffen das!
#include "Application.h"
#include "../Util/Math/Math.h"
#include "../Render/OpenGL/GLShader.h"
#include "../Render/OpenGL/GLTexture.h"

void errorCallback(int error, const char *description) {
    std::cerr << "[GLFW]/[Error] " << error << ": " << description << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    InputManager::updateKey(window, key, scancode, action, mods);
}

void mouseCallback(GLFWwindow *window, double xPos, double yPos) {
    InputManager::updateMouse(window, xPos, yPos);
}

Application::Application(bool isOGL, int width, int height, const char *name) : isOGL(isOGL), width(width), height(height), name(name) {
    if (isOGL) {
        std::cout << "[BakeEngine]:[Info] Starting OpenGL Application ..." << std::endl;
    } else {
        std::cout << "[BakeEngine]:[Info] Starting Vulkan Application ..." << std::endl;
        if (!glfwVulkanSupported()) {
            std::cerr << "[BakeEngine]:[Fatal Error] Vulkan is not supported on this device." << std::endl;
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

    // Creates Application
    window = glfwCreateWindow(width, height, name, nullptr, nullptr);

    // exit when Application was not initialized properly
    if (window == nullptr) {
        std::cerr << "[BakeEngine]:[Error] Error while creating Application context" << std::endl;
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

    // 3f - position (x,y,z) | 3f - color (r,g,b) | 2f - texCoords (u,v) | 3f - normal (x,y,z) | each row a vertex
    float vertices[] = {
            -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f,

            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f,
    };

    unsigned int elementArray[] = {
            0, 1, 2,
            2, 3, 0,

            0, 1, 4,
            4, 5, 1,

            1, 5, 2,
            2, 5, 6,

            6, 2, 3,
            3, 6, 7,

            7, 4, 3,
            3, 0, 4,

            4, 5, 6,
            6, 7, 4
    };
    renderer.addModel("../resources/standard.shader", vertices, sizeof(vertices)/sizeof(float), elementArray, sizeof(elementArray)/sizeof(unsigned int));

    // Vertex Array Object
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element buffer object
    unsigned int ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementArray), elementArray, GL_STATIC_DRAW);

    GLShader shader("../resources/standard.shader");
    shader.bind();

    GLTexture tex(GL_LINEAR, GL_REPEAT, "../resources/texture1.png", TEXTURE_IMAGE);
    tex.bind(0);
    shader.uniform1i("textureImage1", 0);

    // vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // vertex color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coordinate
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    // vertex normals
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(8*sizeof(float)));
    glEnableVertexAttribArray(3);

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

    shader.uniform3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.uniform3f("dirLight.ambient", 0.1f, 0.1f, 0.1f);
    shader.uniform3f("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader.uniform3f("dirLight.specular", 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    Matrix4 model = Matrix4::identity();
    Matrix4 view = Matrix4::identity();
    Matrix4 projection = Matrix4::perspective(Math::toRadians(45), ((float)width)/((float)height), 0.1f, 1000.0f);

    // TODO: Frame skip, show FPS
    long long begin = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    unsigned int refreshRate = 1000000000/100;

    int elementCount = sizeof(elementArray)/sizeof(unsigned int);
    shader.uniformMatrix4fv("projection", projection);
    shader.uniform3f("material.ambient", 1.0f, 1.0f, 1.0f);
    shader.uniform3f("material.diffuse", 1.0f, 1.0f, 1.0f);
    shader.uniform3f("material.specular", 0.5f, 0.5f, 0.5f);
    shader.uniform1f("material.roughness", 32.0f);

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
            if (wNew != width || hNew != height) {
                width = wNew;
                height = hNew;
                projection = Matrix4::perspective(Math::toRadians(45), ((float)width)/((float)height), 0.1f, 1000.0f);
                shader.uniformMatrix4fv("projection", projection);
            }
            getInput(deltaTime);

            view = camera.getView();
            shader.uniform3f("cameraPos", camera.posX(), camera.posY(), camera.posZ());

            shader.uniformMatrix4fv("view", view);

            shader.uniformMatrix4fv("model", model);

            glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnableVertexAttribArray(vao);
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, nullptr);

            glfwGetFramebufferSize(window, &width, &height);
            // glfwSetFrameBufferSizeCallback for future use
            glViewport(0, 0, width, height);

            glfwSwapBuffers(window);
            glfwPollEvents();

            long long frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - begin;
            if (refreshRate - frameTime > 0) {
                std::this_thread::sleep_for(std::chrono::nanoseconds(refreshRate - frameTime));
            }
        }
    }
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vao);
    glfwSetWindowShouldClose(window, 0);
}

void Application::runVk() {

}

void Application::getInput(float deltaTime) {
    camera.speed = 2.5f * deltaTime;
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
