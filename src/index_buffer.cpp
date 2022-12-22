#include "index_buffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) : m_Count(count) {
    CALL_GL(glGenBuffers(1, &m_RendererID));
    CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    CALL_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         count * sizeof(unsigned int),
                         data,
                         GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    CALL_GL(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const {
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const {
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
