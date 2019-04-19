#pragma once
#define BIND_START
#define BIND_DIC
#define BIND_FUNCTION static
#define BIND_CONST(raw_name) static const
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

BIND_START
BIND_DIC class gl
{
public:
	BIND_FUNCTION void test();

	BIND_FUNCTION void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

	BIND_FUNCTION void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

	BIND_FUNCTION void glClear(GLbitfield mask);

	BIND_CONST(GL_COLOR_BUFFER_BIT) GLuint _GL_COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT;


};