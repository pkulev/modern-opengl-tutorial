#include "opengl.hpp"

const char* ErrorToString(GLenum error)
{
    switch (error)
    {
    case GL_NO_ERROR:
        return "No error has been recorded. The value of this symbolic constant is guaranteed to be 0.";

    case GL_INVALID_ENUM:
        return "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";

    case GL_INVALID_VALUE:
        return "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";

    case GL_INVALID_OPERATION:
        return "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";

    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.";

    case GL_OUT_OF_MEMORY:
        return "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
    }

    return "";
}

void PrintGLerrors(const char* function, const char* file, int line)
{
    for (GLenum error = glGetError(); error != GL_NO_ERROR; error = glGetError())
    {
        const char* error_string = ErrorToString(error);
        if (function)
        {
            printf("%s(): 0x%x (%s) at %s:%d\n", function, error, error_string, file, line);
        }
        else
        {
            printf("OpenGL error: 0x%x (%s) at %s:%d\n", error, error_string, file, line);
        }
    }
}
