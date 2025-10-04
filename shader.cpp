#include "shader.h"

#include <fstream>

static std::string ReadFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    std::stringstream source;
    source << file.rdbuf();
    return source.str();
}

ShaderSource & ShaderSource::operator=(ShaderSource &&other) noexcept {
    if (this != &other) {
        // Clean up current resource
        if (m_RendererId != 0) {
            glDeleteShader(m_RendererId);
        }
        // Transfer ownership
        m_RendererId = other.m_RendererId;
        other.m_RendererId = 0;
    }
    return *this;
}

ShaderSource::~ShaderSource() {
    if (m_RendererId == 0) return;
    glDeleteShader(m_RendererId);
}

std::expected<ShaderSource, std::string> ShaderSource::create(ShaderType type, const std::filesystem::path &path) {
    uint32_t shader = glCreateShader(static_cast<GLenum>(type));

    const std::string source = ReadFile(path);
    const char *sourceCStr = source.c_str();

    glShaderSource(shader, 1, &sourceCStr, nullptr);
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

    return ShaderSource(shader);
}

Shader & Shader::operator=(Shader &&other) noexcept {
    if (this != &other) {
        // Clean up current resource
        if (m_RendererId != 0) {
            glDeleteProgram(m_RendererId);
        }
        // Transfer ownership
        m_RendererId = other.m_RendererId;
        other.m_RendererId = 0;
    }
    return *this;
}
void
Shader::Use() const
{
    glUseProgram(m_RendererId);
}

Shader::~Shader() {
    if (m_RendererId == 0) return;
    glDeleteProgram(m_RendererId);
}
