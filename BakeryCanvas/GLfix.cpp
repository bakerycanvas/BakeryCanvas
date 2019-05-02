#include "Bind_GL.h"



static void _glReadnPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data)
{
    int bytes = getByteFromTexel(format, type);
    if (bufSize < bytes*width*height)
    {
        _glSetError(GL_INVALID_OPERATION);
        return;
    }
    glReadPixels(x, y, width, height, format, type, data);
}

//add some functions for lower openGL version
void InitExtraGL()
{
    if (glReadnPixels == NULL)
    {
        glad_glReadnPixels = _glReadnPixels;
    }
}