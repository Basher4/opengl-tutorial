#pragma once

#include <concepts>
#include <ranges>
#include <span>

#include "GL/glew.h"

class VertexBuffer {
public:
    template<typename Container>
    VertexBuffer(const Container& data);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    size_t ElementSize() const { return m_ElemSize; }

private:
    uint32_t m_RendererId = 0;
    size_t m_ElemSize = 0;
};

template<typename Container>
VertexBuffer::VertexBuffer(const Container& data) {
    std::span<const Container::value_type> span_data(data);
    m_ElemSize = sizeof(Container::value_type);

    glGenBuffers(1, &m_RendererId);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
    glBufferData(GL_ARRAY_BUFFER, span_data.size_bytes(), span_data.data(), GL_STATIC_DRAW);
}
