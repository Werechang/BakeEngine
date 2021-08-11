// Wir haben so vieles geschafft - wir schaffen das!
#include "Application.h"
#include "../Render/OpenGL/GLShader.h"
#include "../Render/OpenGL/GLTexture.h"

void errorCallback(int error, const char *description) {
    std::cerr << "GL Error (" << error << "): " << description << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    GLKeys::update(window, key, scancode, action, mods);
}

Application::Application(bool isOGL, int width, int height, const char *name) : isOGL(isOGL), width(width), height(height), name(name) {
    if (isOGL) {
        std::cout << "Starting OpenGL Application ..." << std::endl;
    } else {
        std::cout << "Starting Vulkan Application ..." << std::endl;
        if (!glfwVulkanSupported()) {
            std::cerr << "Vulkan is not supported on this device." << std::endl;
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
        std::cerr << "Error while creating Application context" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    // usually 0 but 1 is better for syncing reasons. On fast computers, buffers may be swapped
    // in the middle of a frame, leading to tearing.
    glfwSwapInterval(1);

    // Function for inputs
    glfwSetKeyCallback(window, keyCallback);

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

    // 3f - position (x,y,z) | 3f - color (r,g,b) | 2f - texCoords (u,v) | each row a vertex
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f
    };

    unsigned int elementArray[] = {
            0, 1, 2,
            2, 3, 0
    };

    // Vertex Array Object
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Element buffer object
    unsigned int ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementArray), elementArray, GL_STATIC_DRAW);

    unsigned int vertexBuffer;

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLShader shader("../resources/standard.shader");
    shader.bind();

    GLTexture tex(GL_NEAREST, GL_REPEAT, "../resources/texture1.png");
    tex.bind(0);
    shader.uniform1i("tex", 0);

    int vertPosAttrib = shader.getAttribLocation("position");
    glEnableVertexAttribArray(vertPosAttrib);
    glVertexAttribPointer(vertPosAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), nullptr);

    int colorAttrib = shader.getAttribLocation("color");
    glEnableVertexAttribArray(colorAttrib);
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));

    int texCoordAttrib = shader.getAttribLocation("texCoord");
    glEnableVertexAttribArray(texCoordAttrib);
    glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));


    Matrix4 view = Matrix4();

    Matrix4 projection = Matrix4();

    Matrix4 model = Matrix4();

    model.translate(1, 10, 2);

    // TODO: Frame skip, show FPS
    long long begin = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    unsigned int refreshRate = 1000000000/60;

    while (!glfwWindowShouldClose(window)) {

        // Time
        long long now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (now - begin >= refreshRate) {
            begin = now;

            Matrix4 mvp = projection * view * model;
            shader.uniformMatrix4fv("mvp", mvp);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glDrawElements(GL_TRIANGLES, sizeof(elementArray)/sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);

            glfwGetFramebufferSize(window, &width, &height);
            // glfwSetFrameBufferSizeCallback for future use
            glViewport(0, 0, width, height);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vao);
    glfwSetWindowShouldClose(window, 0);
}

void Application::runVk() {

}
