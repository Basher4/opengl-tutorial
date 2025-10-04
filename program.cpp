#include "program.h"

#include <fstream>

static std::string ReadFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    std::stringstream source;
    source << file.rdbuf();
    return source.str();
}

Shader & Shader::operator=(Shader &&other) noexcept {
    if (this != &other) {
        // Clean up current resource
        if (m_Id != 0) {
            glDeleteShader(m_Id);
        }
        // Transfer ownership
        m_Id = other.m_Id;
        other.m_Id = 0;
    }
    return *this;
}

Shader::~Shader() {
    if (m_Id == 0) return;
    glDeleteShader(m_Id);
}

std::expected<Shader, std::string> Shader::create(ShaderType type, const std::filesystem::path &path) {
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

    return Shader(shader);
}

Program & Program::operator=(Program &&other) noexcept {
    if (this != &other) {
        // Clean up current resource
        if (m_Id != 0) {
            glDeleteProgram(m_Id);
        }
        // Transfer ownership
        m_Id = other.m_Id;
        other.m_Id = 0;
    }
    return *this;
}

Program::~Program() {
    if (m_Id == 0) return;
    glDeleteProgram(m_Id);
}
