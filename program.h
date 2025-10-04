#pragma once

#include <expected>
#include <filesystem>
#include <iostream>
#include <valarray>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum class ShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    TesselationControl = GL_TESS_CONTROL_SHADER,
    Compute = GL_COMPUTE_SHADER,
};

class Shader {
private:
    explicit Shader(uint32_t id) : m_Id(id) {};

public:
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader& operator=(Shader&& other) noexcept;
    Shader(Shader&& other) noexcept : m_Id(other.m_Id) {
        other.m_Id = 0; // Invalidate the moved-from object
    }

public:
    static std::expected<Shader, std::string> create(ShaderType type,  const std::filesystem::path &path);
    uint32_t id() const { return m_Id; }

private:
    uint32_t m_Id;
};

class Program {
private:
    explicit Program(uint32_t id) : m_Id(id) {};

public:
    ~Program();

    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;

    Program& operator=(Program&& other) noexcept;
    Program(Program&& other) noexcept : m_Id(other.m_Id) {
        other.m_Id = 0; // Invalidate the moved-from object
    }

public:
    template <typename... Ts>
    requires (std::same_as<Ts, Shader> && ...)
    static std::expected<Program, std::string> create(Ts&&... shaders);

    uint32_t id() const { return m_Id; }

private:
    uint32_t m_Id;
};

template<typename ... Ts> requires (std::same_as<Ts, Shader> && ...)
std::expected<Program, std::string> Program::create(Ts &&...shaders) {
    uint32_t id = glCreateProgram();

    // Store shaders in a tuple to keep them alive until linking completes
    auto shader_tuple = std::make_tuple(std::forward<Ts>(shaders)...);
    std::apply([id](auto&... shader_refs) {
        (glAttachShader(id, shader_refs.id()), ...);
    }, shader_tuple);

    glLinkProgram(id);
    glValidateProgram(id);

    return Program(id);
}
