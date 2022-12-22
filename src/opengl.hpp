#pragma once

#include <stdio.h>
#include <GL/glew.h>

#define CALL_GL(expr)                         \
    expr;                                     \
    PrintGLerrors(#expr, __FILE__, __LINE__); \

const char *ErrorToString(GLenum error);
void PrintGLerrors(const char* function, const char* file, int line);
