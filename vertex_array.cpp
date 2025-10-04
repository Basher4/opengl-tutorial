//
// Created by user on 10/4/2025.
//

#include "vertex_array.h"

#include <cassert>

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_RendererId);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_RendererId);
}
void
VertexArray::Bind() const
{
    glBindVertexArray(m_RendererId);
}
void
VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer &buffer, std::initializer_list<VertexBufferElement> layout) const
{
    Bind();
    buffer.Bind();

    for (auto const [idx, l] : std::views::enumerate(layout)) {
        glEnableVertexAttribArray(idx);

        assert(l.count > 0 && l.count <= 4);
        glVertexAttribPointer(idx, static_cast<GLint>(l.count), static_cast<GLenum>(l.type),
            GL_FALSE, buffer.ElementSize(), reinterpret_cast<void *>(l.offset));
    }
}
