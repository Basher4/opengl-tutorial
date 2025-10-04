#include "shader.h"

#include <fstream>

static std::string ReadFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    std::stringstream source;
    source << file.rdbuf();
    return source.str();
}

Shader::~Shader()
{
    if (m_RendererId == 0)
        return;
    glDeleteProgram(m_RendererId);
}

void Shader::Bind() const { glUseProgram(m_RendererId); }
void Shader::Unbind() const { glUseProgram(0); }

std::expected<Shader, std::string> Shader::create(
    const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path)
{
    auto fragment = ShaderSource::create(ShaderType::Fragment, fragment_path);
    auto vertex = ShaderSource::create(ShaderType::Vertex, vertex_path);

    if (!fragment || !vertex) {
        std::stringstream ss;
        if (!fragment) {
            ss << fragment_path << ":\n" << fragment.error() << '\n';
        }

        if (!vertex) {
            ss << vertex_path << ":\n" << vertex.error() << '\n';
        }

        return std::unexpected(ss.str());
    }

    uint32_t id = glCreateProgram();
    glAttachShader(id, fragment->id());
    glAttachShader(id, vertex->id());
    glLinkProgram(id);
    glValidateProgram(id);

    return Shader(id);
}

Shader& Shader::operator=(Shader&& other) noexcept
{
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

Shader::ShaderSource::~ShaderSource()
{
    if (m_RendererId == 0)
        return;
    glDeleteShader(m_RendererId);
}

std::expected<Shader::ShaderSource, std::string> Shader::ShaderSource::create(
    ShaderType type, const std::filesystem::path& path)
{
    uint32_t shader = glCreateShader(static_cast<GLenum>(type));

    const std::string source = ReadFile(path);
    const char* sourceCStr = source.c_str();

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

Shader::ShaderSource& Shader::ShaderSource::operator=(ShaderSource&& other) noexcept
{
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
