#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "fpsCounter.hpp"
#include "camera.hpp"
#include "world.hpp"
#include "inputs.hpp"
#include "hud.hpp"
#include "texturemanip/atlas.hpp"

int main() {
    // Check block ID configuration
    ensureCorrectBlockIDs();

    if (!glfwInit()) {
        std::cerr << "Error during GLFW initialization." << std::endl;
        return 1;
    }

    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "[GLFW Error]: " << description << std::endl;
    });

    // Creating a window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(960, 720, "Voxels !", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW: window or OpenGL context creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Loading OpenGL function pointers
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

    // Print useful info
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Setting up OpenGL viewport and other settings
    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    glClearColor(0.2, 0.2, 0.2, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapInterval(1);

    // Instantiate FPS counter
    FpsCounter fpsCounter(window, 0.5);

    // Instantiate camera
    Camera camera(window);
    camera.position = glm::vec3(0.0, 13.0, 0.0);

    // Instantiate world and hud
    World world;
    Hud hud(window);

    // Instantiate input manager
    InputManager input(window, world, camera, hud);
    glfwSetWindowUserPointer(window, &input);

    // Register GLFW callbacks
    glfwSetKeyCallback(window, EventCallbacks::onKey);
    glfwSetFramebufferSizeCallback(window, EventCallbacks::onFrameBufferResize);
    glfwSetCursorPosCallback(window, EventCallbacks::onCursorMove);
    glfwSetScrollCallback(window, EventCallbacks::onScroll);
    glfwSetMouseButtonCallback(window, EventCallbacks::onMouseButton);

    // Cursor settings
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    } else {
        std::cout << "[WARN] Raw mouse motion is not available on this system." << std::endl;
    }

    Atlas atlas("assets/textures/atlas.png", GL_TEXTURE0);
    atlas.registerTextureUV("test", {0, 0, 16, 16});
    atlas.registerTextureUV("stone", {0, 16, 16, 16});
    atlas.registerTextureUV("grass_top", {16, 16, 16, 16});
    atlas.registerTextureUV("grass_sides", {16, 0, 16, 16});

    while (!glfwWindowShouldClose(window)) {
        fpsCounter.frameBegin();
        double deltaTime = fpsCounter.getLastFrameDuration();
        camera.processInputs(window, static_cast<float>(deltaTime));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world.draw(camera, atlas);

        glClear(GL_DEPTH_BUFFER_BIT); // We want hud elements to always be drawn on top of world elements

        hud.draw();

        fpsCounter.frameDone();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
