#pragma once
#include <span>

#include "GL/glew.h"

class IndexBuffer {
public:
    template<typename Container>
    IndexBuffer(const Container& data);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    uint32_t Count() const;
private:
    uint32_t m_RendererId = 0;
    uint32_t m_Count = 0;
};

template<typename Container>
IndexBuffer::IndexBuffer(const Container& data) {
    std::span<const Container::value_type> span_data(data);
    m_Count = span_data.size();

    glGenBuffers(1, &m_RendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, span_data.size_bytes(), span_data.data(), GL_STATIC_DRAW);
}
