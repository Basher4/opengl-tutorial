#pragma once

#include "vertex_buffer.h"

enum class VertexAttribType {
    Float = GL_FLOAT,
};

struct VertexBufferElement {
    VertexBufferElement(uint8_t count, VertexAttribType type, uint32_t offset) : count(count), type(type), offset(offset) {}

    uint8_t count;   // Must be 1, 2, 3, or 4.
    VertexAttribType type;
    uint32_t offset;
};

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddBuffer(const VertexBuffer& buffer, std::initializer_list<VertexBufferElement> layout) const;

private:
    uint32_t m_RendererId = 0;
};
