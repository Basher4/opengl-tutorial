#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static std::string ReadFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    std::stringstream source;
    source << file.rdbuf();
    return source.str();
}

static std::expected<uint32_t, std::string> CreateShader(const uint32_t type, const char* source) {
    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        int length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::string message;
        message.resize(length);
        glGetShaderInfoLog(shader, length, &length, const_cast<char*>(message.c_str()));
        glDeleteShader(shader);

        return std::unexpected(std::string(message));
    }

    return shader;
}

static std::expected<uint32_t, std::string> CompileShader(const std::filesystem::path& vertexShader, const std::filesystem::path& fragmentShader) {
    GLuint program = glCreateProgram();

    std::string vertexSoruce = ReadFile(vertexShader);
    std::string fragmentSource = ReadFile(fragmentShader);

    auto vid = CreateShader(GL_VERTEX_SHADER, vertexSoruce.c_str());
    if (!vid) {
        auto error = std::format("Failed to compile vertex shader '{}':\n{}", vertexShader.string(), vid.error());
        return std::unexpected(error);
    }

    auto fid = CreateShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());;
    if (!fid) {
        auto error = std::format("Failed to compile fragment shader '{}':\n{}", fragmentShader.string(), fid.error());
        return std::unexpected(error);
    }

    glAttachShader(program, *vid);
    glAttachShader(program, *fid);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(*vid);
    glDeleteShader(*fid);

    return program;
}

typedef struct {
    float x, y;
    float r, g, b;
} Vertex;

static void GLAPIENTRY
GlDebugMessageCb([[maybe_unused]] GLenum _source,
                 [[maybe_unused]] GLenum type,
                 GLuint id,
                 GLenum severity,
                 [[maybe_unused]] GLsizei length,
                 const GLchar *message,
                 const void *userParam) {
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
        glfwSetWindowShouldClose((GLFWwindow*)userParam, 1);
    }
}

using Callback = GLDEBUGPROC;

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW3" << std::endl;
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(640, 480, "OpenGL Demo", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to open GLFW3" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "Failed to initialize GLEW: " << err << std::endl;
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GlDebugMessageCb, window);

    auto shader = CompileShader("./shaders/shader.vert", "./shaders/shader.frag");
    if (!shader) {
        std::cout << "Error: " << shader.error() << std::endl;
        return -1;
    }

    Vertex vertices[] = {
        { -0.5f, -0.5f, 1.0f, 0.0f, 0.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
        {  0.5f,  0.5f, 0.0f, 0.0f, 1.0f },
        { -0.5f,  0.5f, 1.0f, 1.0f, 1.0f },
    };

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

    glUseProgram(*shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(*shader);

    glfwTerminate();
    return 0;
}
