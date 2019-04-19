#include "GL.h"

void gl::test()
{
	printf("test\n");
}

void gl::glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	::glViewport(x, y, width, height);
}

void gl::glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	::glClearColor(red, green, blue, alpha);
}

void gl::glClear(GLbitfield mask)
{
	::glClear(mask);
}

