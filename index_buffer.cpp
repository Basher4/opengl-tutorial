#include "index_buffer.h"

#include "GL/glew.h"

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_RendererId);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
