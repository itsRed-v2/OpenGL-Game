#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "fpsCounter.hpp"
#include "camera.hpp"
#include "world.hpp"
#include "inputs.hpp"
#include "hud.hpp"
#include "texturemanip/atlas.hpp"
#include "logger.hpp"

int main() {
    // Check block ID configuration
    ensureCorrectBlockIDs();

    if (!glfwInit()) {
        Logger::crash("Error during GLFW initialization.");
    }

    glfwSetErrorCallback([](int error, const char* description) {
        Logger::error("[GLFW Error]: " + std::string(description));
    });

    // Creating a window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(960, 720, "Voxels !", nullptr, nullptr);
    if (!window) {
        Logger::crash("GLFW: window or OpenGL context creation failed.");
    }
    glfwMakeContextCurrent(window);

    // Loading OpenGL function pointers
    if (!gladLoadGL(glfwGetProcAddress)) {
        Logger::crash("Failed to initialize GLAD");
    }

    // Print useful info
    auto versionString = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    Logger::info("OpenGL version: " + std::string(versionString));

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
        Logger::warn("Raw mouse motion is not available on this system.");
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
