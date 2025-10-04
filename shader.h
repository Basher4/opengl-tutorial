#pragma once

#include <expected>
#include <filesystem>
#include <iostream>
#include <valarray>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader {
private:
    explicit Shader(uint32_t id) : m_RendererId(id) {};

public:
    ~Shader();

    void Bind() const;

    static std::expected<Shader, std::string> create(
        const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path);

    // Copy
    Shader& operator=(const Shader&) = delete;
    Shader(const Shader&) = delete;
    // Move
    Shader& operator=(Shader&& other) noexcept;
    Shader(Shader&& other) noexcept : m_RendererId(other.m_RendererId) {
        other.m_RendererId = 0; // Invalidate the moved-from object
    }

private:
    uint32_t m_RendererId = 0;

private:
    enum class ShaderType {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        TesselationControl = GL_TESS_CONTROL_SHADER,
        Compute = GL_COMPUTE_SHADER,
    };

    class ShaderSource {
        explicit ShaderSource(uint32_t id) : m_RendererId(id) {};

    public:
        ~ShaderSource();

        static std::expected<ShaderSource, std::string> create(ShaderType type,  const std::filesystem::path &path);
        uint32_t id() const { return m_RendererId; }

        // Copy
        ShaderSource(const ShaderSource&) = delete;
        ShaderSource& operator=(const ShaderSource&) = delete;
        // Move
        ShaderSource& operator=(ShaderSource&& other) noexcept;
        ShaderSource(ShaderSource&& other) noexcept : m_RendererId(other.m_RendererId) {
            other.m_RendererId = 0; // Invalidate the moved-from object
        }

    private:
        uint32_t m_RendererId = 0;
    };
};
