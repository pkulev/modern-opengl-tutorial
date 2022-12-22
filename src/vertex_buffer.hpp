#pragma once

#include <GL/glew.h>

#include "opengl.hpp"


class VertexBuffer {
public:
    VertexBuffer(const void *data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
private:
    GLuint m_RendererID;
};
