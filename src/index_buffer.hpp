#pragma once

#include <GL/glew.h>

#include "opengl.hpp"


class IndexBuffer {
public:
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return m_Count; }
private:
    GLuint m_RendererID;
    unsigned int m_Count;
};
