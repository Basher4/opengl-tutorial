#include <array>
#include <expected>
#include <filesystem>
#include <format>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "app.h"
#include "index_buffer.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

typedef struct {
    float x, y;
    float r, g, b;
} Vertex;

template<typename T>
T EXPECT(std::expected<T, std::string> result, std::string fail) {
    if (!result) {
        std::cerr << fail << '\n' << result.error() << std::endl;
        exit(-1);
    }

    return std::move(*result);
}

int main() {
    App app(640, 480);
    app.Init();

    auto shader = EXPECT(Shader::create("./shaders/shader.vert", "./shaders/shader.frag"),
                         "Failed to create shaders");

    constexpr auto vertices = std::to_array<Vertex>({
        { -0.5f, -0.5f, 1.0f, 0.0f, 0.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
        {  0.5f,  0.5f, 0.0f, 0.0f, 1.0f },
        { -0.5f,  0.5f, 1.0f, 1.0f, 1.0f },
    });

    VertexArray va;
    VertexBuffer vb(vertices);
    va.AddBuffer(vb, {
        VertexBufferElement(2, VertexAttribType::Float, 0),
        VertexBufferElement(3, VertexAttribType::Float, offsetof(Vertex, r)),
    });

    constexpr auto indices = std::to_array<uint32_t>({
        0, 1, 2,
        2, 3, 0,
    });
    IndexBuffer ibo(indices);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    shader.Bind();

    /* Loop until the user closes the window */
    float time = 0.0;
    while (!glfwWindowShouldClose(app.Window()))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(0, time);

        va.Bind();
        ibo.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(app.Window());

        /* Poll for and process events */
        glfwPollEvents();

        time += 0.05;
    }

    return 0;
}
