#pragma once

#include <expected>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class App {
public:
    App(int window_w, int window_h, bool vsync = true)
        : m_WindowX(window_w), m_WindowY(window_h), m_VSync(vsync) {}
    ~App();

    std::expected<int, std::string> Init();

    GLFWwindow* Window() const { return m_Window; }

private:
    int m_WindowX = 0, m_WindowY = 0;
    bool m_VSync = true;
    GLFWwindow *m_Window;
};
