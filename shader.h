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

class ShaderSource {
private:
    explicit ShaderSource(uint32_t id) : m_RendererId(id) {};

public:
    ~ShaderSource();

    ShaderSource(const ShaderSource&) = delete;
    ShaderSource& operator=(const ShaderSource&) = delete;

    ShaderSource& operator=(ShaderSource&& other) noexcept;
    ShaderSource(ShaderSource&& other) noexcept : m_RendererId(other.m_RendererId) {
        other.m_RendererId = 0; // Invalidate the moved-from object
    }

public:
    static std::expected<ShaderSource, std::string> create(ShaderType type,  const std::filesystem::path &path);
    uint32_t id() const { return m_RendererId; }

private:
    uint32_t m_RendererId = 0;
};

class Shader {
private:
    explicit Shader(uint32_t id) : m_RendererId(id) {};

public:
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader& operator=(Shader&& other) noexcept;
    Shader(Shader&& other) noexcept : m_RendererId(other.m_RendererId) {
        other.m_RendererId = 0; // Invalidate the moved-from object
    }

public:
    template <typename... Ts>
    requires (std::same_as<Ts, ShaderSource> && ...)
    static std::expected<Shader, std::string> create(Ts&&... shaders);

    void Use() const;

    uint32_t id() const { return m_RendererId; }

private:
    uint32_t m_RendererId = 0;
};

template<typename ... Ts> requires (std::same_as<Ts, ShaderSource> && ...)
std::expected<Shader, std::string> Shader::create(Ts &&...shaders) {
    uint32_t id = glCreateProgram();

    // Store shaders in a tuple to keep them alive until linking completes
    auto shader_tuple = std::make_tuple(std::forward<Ts>(shaders)...);
    std::apply([id](auto&... shader_refs) {
        (glAttachShader(id, shader_refs.id()), ...);
    }, shader_tuple);

    glLinkProgram(id);
    glValidateProgram(id);

    return Shader(id);
}
