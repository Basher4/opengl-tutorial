#include "vertex_buffer.h"

#include "GL/glew.h"

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendererId);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
