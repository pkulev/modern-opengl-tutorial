#include "vertex_buffer.hpp"

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
    CALL_GL(glGenBuffers(1, &m_RendererID));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    CALL_GL(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const {
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const {
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
