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
#ifdef BK_ENABLE_SHADER_TRANSLATOR
#include "translator.h"
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <set>
#include <regex>

//define extra webGL macro
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9
#define GL_RGB565 0x8D62
#define GL_LOW_FLOAT 0x8DF0
#define GL_MEDIUM_FLOAT 0x8DF1
#define GL_HIGH_FLOAT 0x8DF2
#define GL_LOW_INT 0x8DF3
#define GL_MEDIUM_INT 0x8DF4
#define GL_HIGH_INT 0x8DF5 
#define GL_MAX_VERTEX_UNIFORM_VECTORS 0x8DFB
#define GL_MAX_VARYING_VECTORS 0x8DFC
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS 0x8DFD
#define GL_UNPACK_FLIP_Y_WEBGL 0x9240
#define GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL 0x9241
#define GL_CONTEXT_LOST_WEBGL 0x9242
#define GL_UNPACK_COLORSPACE_CONVERSION_WEBGL 0x9243
#define GL_BROWSER_DEFAULT_WEBGL 0x9244

#ifdef BK_ENABLE_SHADER_TRANSLATOR
#define ANGLE_HASH_NAME_PREFIX "_u"
#endif

#if defined(DEBUG) || defined(_DEBUG) || defined(BKE_DEBUG)
#define CHECK_GL {auto x=glGetError();if(x!=0)printf("%s:%d, %04X\n", __FUNCTION__,__LINE__,x);}
#else
#define CHECK_GL
#endif

void Bind_GL(v8::Isolate * iso);

v8::Local<v8::Object> getGLmodule();

std::string mapShader(const char *src, GLenum shaderType);

bool shouldSwapBuffer();

void clearSwapBufferTag();

void setSwapBufferTag();

struct WebGLContextAttributes;

WebGLContextAttributes* getCurrentContextAttributes();

struct WebGLContextAttributes
{
    enum
    {
        PERFORMANCE_DEFAULT,
        PERFORMANCE_HIGH,
        PERFORMANCE_LOW
    };
    bool alpha = true;
    bool depth = true;
    bool stencil = false;
    bool antialias = true;
    bool premultipliedAlpha = true;
    bool preserveDrawingBuffer = false;
    int powerPreference = PERFORMANCE_DEFAULT;
    bool failIfMajorPerformanceCaveat = false;
    bool desynchronized = false;
};

class WebGLObject
{
    GLboolean invalidated;
public:
    WebGLObject()
    {
        invalidated = false;
    }

    void Invalidate()
    {
        invalidated = true;
    }

    bool isValid() const
    {
        return !invalidated;
    }
};

class WebGLBuffer : public WebGLObject
{
public:
    GLuint buffer;

    ~WebGLBuffer()
    {
        if (isValid())
            glDeleteBuffers(1, &buffer);
    }
};

class WebGLFramebuffer : public WebGLObject
{
public:
    GLuint fbo;

    ~WebGLFramebuffer()
    {
        if (isValid())
            glDeleteFramebuffers(1, &fbo);
    }
};

class WebGLProgram : public WebGLObject
{
public:
    GLuint program;

    ~WebGLProgram()
    {
        if (isValid())
            glDeleteProgram(program);
    }
};

class WebGLRenderbuffer : public WebGLObject
{
public:
    GLuint rbo;

    ~WebGLRenderbuffer()
    {
        if (isValid())
            glDeleteRenderbuffers(1, &rbo);
    }
};

class WebGLShader : public WebGLObject
{
public:
    GLuint shader;
    //cache shader type
    GLenum type;

    ~WebGLShader()
    {
        if (isValid())
            glDeleteShader(shader);
    }
};

class WebGLTexture : public WebGLObject
{
public:
    GLuint textureID;

    ~WebGLTexture()
    {
        if(isValid())
            glDeleteTextures(1, &textureID);
    }
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