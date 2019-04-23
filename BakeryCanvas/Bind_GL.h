#pragma once

#include "v8pp/config.hpp"
#include "v8pp/context.hpp"
#include "v8pp/call_v8.hpp"
#include "v8pp/call_from_v8.hpp"
#include "v8pp/class.hpp"
#include "v8pp/json.hpp"
#include "v8pp/module.hpp"
#include "v8pp/object.hpp"
#include "v8pp/persistent.hpp"
#include "v8pp/throw_ex.hpp"
#include "v8pp/utility.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <set>

void Bind_GL(v8::Isolate * iso);

class WebGLObject
{
public:
	GLboolean invalidated;

	WebGLObject()
	{
		invalidated = false;
	}
};

class WebGLBuffer : public WebGLObject
{
public:
	GLuint buffer;
};

class WebGLFramebuffer : public WebGLObject
{
public:
	GLuint fbo;
};

class WebGLProgram : public WebGLObject
{
public:
	GLuint program;
};

class WebGLRenderbuffer : public WebGLObject
{
public:
	GLuint rbo;
};

class WebGLShader : public WebGLObject
{
public:
	GLuint shader;
};

class WebGLTexture : public WebGLObject
{
public:
    GLuint textureID;
};

class WebGLUniformLocation : public WebGLObject
{
public:
    GLuint location;
};

class WebGLActiveInfo : public WebGLObject
{
public:
	GLint size;
	GLenum type;
	std::string name;

	GLint getSize() const
	{
		return size;
	}

	GLenum getType() const
	{
		return type;
	}

	std::string getName() const
	{
		return name;
	}
};

class WebGLShaderPrecisionFormat : public WebGLObject
{
public:
	GLint rangeMin;
	GLint rangeMax;
	GLint precision;

	GLint getRangeMin() const
	{
		return rangeMin;
	}

	GLint getRangeMax() const
	{
		return rangeMax;
	}

	GLint getPrecision() const
	{
		return precision;
	}
};