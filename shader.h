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
    void Unbind() const;

    template<typename... T>
    void SetUniform(int slot, T... value) { static_assert(false, "Not implemented"); }

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

template <> inline void Shader::SetUniform(int slot, float v1) { glUniform1f(slot, v1); }
template <> inline void Shader::SetUniform(int slot, float v1, float v2) { glUniform2f(slot, v1, v2); }
template <> inline void Shader::SetUniform(int slot, float v1, float v2, float v3) { glUniform3f(slot, v1, v2, v3); }
template <> inline void Shader::SetUniform(int slot, float v1, float v2, float v3, float v4) { glUniform4f(slot, v1, v2, v3, v4); }
template <> inline void Shader::SetUniform(int slot, int v1) { glUniform1i(slot, v1); }
template <> inline void Shader::SetUniform(int slot, int v1, int v2) { glUniform2i(slot, v1, v2); }
template <> inline void Shader::SetUniform(int slot, int v1, int v2, int v3) { glUniform3i(slot, v1, v2, v3); }
template <> inline void Shader::SetUniform(int slot, int v1, int v2, int v3, int v4) { glUniform4i(slot, v1, v2, v3, v4); }
template <> inline void Shader::SetUniform(int slot, uint32_t v1) { glUniform1ui(slot, v1); }
template <> inline void Shader::SetUniform(int slot, uint32_t v1, uint32_t v2) { glUniform2ui(slot, v1, v2); }
template <> inline void Shader::SetUniform(int slot, uint32_t v1, uint32_t v2, uint32_t v3) { glUniform3ui(slot, v1, v2, v3); }
template <> inline void Shader::SetUniform(int slot, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4) { glUniform4ui(slot, v1, v2, v3, v4); }
