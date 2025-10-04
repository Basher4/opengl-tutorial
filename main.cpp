#include <expected>
#include <filesystem>
#include <format>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "app.h"
#include "program.h"

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

    auto vert = EXPECT(Shader::create(ShaderType::Vertex, "./shaders/shader.vert"),
                       "Failed to create vertex shader");
    auto frag = EXPECT(Shader::create(ShaderType::Fragment, "./shaders/shader.frag"),
                       "Failed to create fragment shader");
    auto shader = EXPECT(Program::create(std::move(vert), std::move(frag)),
                         "Failed to create shader");

    Vertex vertices[] = {
        { -0.5f, -0.5f, 1.0f, 0.0f, 0.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
        {  0.5f,  0.5f, 0.0f, 0.0f, 1.0f },
        { -0.5f,  0.5f, 1.0f, 1.0f, 1.0f },
    };

    uint32_t vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    uint32_t vertex_buffer = 0;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));


    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    uint32_t ibo = 0;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glUseProgram(shader.id());

    /* Loop until the user closes the window */
    float time = 0.0;
    while (!glfwWindowShouldClose(app.Window()))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(0, time);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(app.Window());

        /* Poll for and process events */
        glfwPollEvents();

        time += 0.05;
    }

    return 0;
}
