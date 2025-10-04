#include "app.h"

#include <cassert>
#include <format>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

App::~App() {
    assert(m_Window != nullptr);
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

static void GLAPIENTRY
GlDebugMessageCb([[maybe_unused]] GLenum source, [[maybe_unused]] GLenum type,
                 GLuint id, GLenum severity, [[maybe_unused]] GLsizei length,
                 const char *message, const void *userParam) {
    const auto app = static_cast<const App*>(userParam);

    switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "[INF";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "[LOW";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "[MED";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "[HIGH";
            break;
    }

    std::cout << "(" << id << ")] " << message << std::endl;

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        glfwSetWindowShouldClose(app->Window(), 1);
    }
}

std::expected<int, std::string> App::Init() {
    if (!glfwInit()) {
        return std::unexpected("Failed to initialize GLFW");
    }

    // It might be cool to eventually allow multiple windows in an app.
    // Will refactor if we actually get there.
    m_Window = glfwCreateWindow(m_WindowX, m_WindowY, "Hello World", NULL, NULL);
    if (!m_Window) {
        glfwTerminate();
        return std::unexpected("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(m_VSync);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        return std::unexpected("Failed to init GLEW");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GlDebugMessageCb, this);

    return 0;
}
