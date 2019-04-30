#include "Bind_GL.h"
#include "translator.h"

#ifdef BK_ENABLE_SHADER_TRANSLATOR
static const std::string variable_prefix = ANGLE_HASH_NAME_PREFIX;
#else
static const std::string variable_prefix = "";
#endif

static GLenum inner_GLError = GL_NO_ERROR;
static GLboolean use_InnerError = false;
static GLboolean glContextLost = false;

static GLuint default_VAO = 0;

//some webGL spercific configuration
const GLboolean const_GL_UNPACK_FLIP_Y_WEBGL = false;
const GLboolean const_GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL = false;
//GL_BROWSER_DEFAULT_WEBGL for sRGB, or GL_NONE for RGB
const GLint const_GL_UNPACK_COLORSPACE_CONVERSION_WEBGL = GL_BROWSER_DEFAULT_WEBGL;

//context
static WebGLContextAttributes GL_context;

//should swap buffer at the end of the frame
static bool GL_shouldSwapBuffer = false;

void _glSetError(GLenum error)
{
    inner_GLError = error;
    use_InnerError = true;
}

GLenum _glGetError() 
{
    if (use_InnerError)
    {
        use_InnerError = false;
        return inner_GLError;
    }
    else
    {
        return glGetError();
    }
}

bool _glIsContextLost()
{
    //suppose never lost on destop system
    return false;
}

void initDefaultVAO()
{
    if (default_VAO == 0)
    {
        glGenVertexArrays(1, &default_VAO);
        glBindVertexArray(default_VAO);
    }
}

#define SETVALUE(x) result->Set(context, v8::String::NewFromUtf8(isolate, #x), v8pp::to_v8(isolate, GL_context.x));

v8::Local<v8::Object> _glGetContextAttributes()
{
    auto isolate = v8::Isolate::GetCurrent();
    v8::EscapableHandleScope scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Object> result = v8::Object::New(isolate);
    SETVALUE(alpha);
    SETVALUE(depth);
    SETVALUE(stencil);
    SETVALUE(antialias);
    SETVALUE(premultipliedAlpha);
    SETVALUE(preserveDrawingBuffer);
    SETVALUE(powerPreference);
    SETVALUE(failIfMajorPerformanceCaveat);
    SETVALUE(desynchronized);
    return scope.Escape(result);
}

//use GLboolean will cause convert error in v8pp when giving a boolean in js
void _glVertexAttribPointer(GLuint index, GLint size, GLenum type, bool normalized, GLsizei stride, intptr_t pointer)
{
    initDefaultVAO();
    glVertexAttribPointer(index, size, type, normalized, stride, (const void*)pointer);
    CHECK_GL;
}

void _glVertexAttrib2fv(GLuint index, intptr_t v)
{
    initDefaultVAO();
    glVertexAttrib2fv(index, (const GLfloat *)v);
    CHECK_GL;
}

void _glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, intptr_t pixels)
{
    glTexImage2D(target, level, internalformat, width, height, border, format, type, (void*)pixels);
    CHECK_GL;
}

v8::Local<v8::Value> _glGetParameter(GLenum pname)
{
    switch (pname)
    { 
    //return int
    case GL_ACTIVE_TEXTURE:
    case GL_ALPHA_BITS:
    case GL_BLEND_DST_ALPHA:
    case GL_BLEND_DST_RGB:
    case GL_BLEND_EQUATION:
    case GL_BLEND_EQUATION_ALPHA:
    //case GL_BLEND_EQUATION_RGB: same as GL_BLEND_EQUATION
    case GL_BLEND_SRC_ALPHA:
    case GL_BLEND_SRC_RGB:
    case GL_BLUE_BITS:
    case GL_CULL_FACE_MODE:
    case GL_DEPTH_BITS:
    case GL_DEPTH_FUNC:
    case GL_FRONT_FACE:
    case GL_GENERATE_MIPMAP_HINT:
    case GL_GREEN_BITS:
    case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
    case GL_MAX_CUBE_MAP_TEXTURE_SIZE:
    case GL_MAX_RENDERBUFFER_SIZE:
    case GL_MAX_TEXTURE_IMAGE_UNITS:
    case GL_MAX_TEXTURE_SIZE:
    case GL_MAX_VERTEX_ATTRIBS:
    case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
    case GL_PACK_ALIGNMENT:
    case GL_RED_BITS:
    case GL_SAMPLE_BUFFERS:
    case GL_SAMPLES:
    case GL_STENCIL_BACK_FAIL:
    case GL_STENCIL_BACK_FUNC:
    case GL_STENCIL_BACK_PASS_DEPTH_FAIL:
    case GL_STENCIL_BACK_PASS_DEPTH_PASS:
    case GL_STENCIL_BACK_REF:
    case GL_STENCIL_BITS:
    case GL_STENCIL_CLEAR_VALUE:
    case GL_STENCIL_FAIL:
    case GL_STENCIL_FUNC:
    case GL_STENCIL_PASS_DEPTH_FAIL:
    case GL_STENCIL_PASS_DEPTH_PASS:
    case GL_STENCIL_REF:
    case GL_SUBPIXEL_BITS:
    case GL_UNPACK_ALIGNMENT:
        {
            GLint value;
            glGetIntegerv(pname, &value);
            CHECK_GL;
            return v8::Int32::New(v8::Isolate::GetCurrent(), value);
        }
        //I don't know where these macros are defined in openGL, but seems it works on openGL
    case GL_MAX_VERTEX_UNIFORM_VECTORS:
    case GL_MAX_VARYING_VECTORS:
    case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
    case GL_IMPLEMENTATION_COLOR_READ_FORMAT:
    case GL_IMPLEMENTATION_COLOR_READ_TYPE:
        {
            GLint value;
            glGetIntegerv(pname, &value);
            CHECK_GL;
            return v8::Int32::New(v8::Isolate::GetCurrent(), value);
        }
        return v8::Null(v8::Isolate::GetCurrent());
    case GL_UNPACK_COLORSPACE_CONVERSION_WEBGL:
        {
            return v8::Int32::New(v8::Isolate::GetCurrent(), const_GL_UNPACK_COLORSPACE_CONVERSION_WEBGL);
        }

    //return uint
    case GL_STENCIL_BACK_VALUE_MASK:
    case GL_STENCIL_BACK_WRITEMASK:
    case GL_STENCIL_VALUE_MASK:
    case GL_STENCIL_WRITEMASK:
        {
            GLuint value;
            glGetIntegerv(pname, (GLint *)&value);
            CHECK_GL;
            return v8::Uint32::New(v8::Isolate::GetCurrent(), value);
        }
    //return boolean
    case GL_BLEND:
    case GL_CULL_FACE:
    case GL_DEPTH_TEST:
    case GL_DEPTH_WRITEMASK:
    case GL_DITHER:
    case GL_POLYGON_OFFSET_FILL:
    case GL_SAMPLE_COVERAGE_INVERT:
    case GL_SCISSOR_TEST:
    case GL_STENCIL_TEST:
        {
            GLboolean value;
            glGetBooleanv(pname, &value);
            CHECK_GL;
            return v8::Boolean::New(v8::Isolate::GetCurrent(), value);
        }
    case GL_UNPACK_FLIP_Y_WEBGL:
        {
            return v8::Boolean::New(v8::Isolate::GetCurrent(), const_GL_UNPACK_FLIP_Y_WEBGL);
        }
    case GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL:
        {
            return v8::Boolean::New(v8::Isolate::GetCurrent(), const_GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL);
        }
    //return float
    case GL_DEPTH_CLEAR_VALUE:
    case GL_LINE_WIDTH:
    case GL_POLYGON_OFFSET_FACTOR:
    case GL_POLYGON_OFFSET_UNITS:
    case GL_SAMPLE_COVERAGE_VALUE:
        {
            GLfloat value;
            glGetFloatv(pname, &value);
            CHECK_GL;
            return v8::Number::New(v8::Isolate::GetCurrent(), value);
        }

    case GL_VERSION:
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "WebGL 1.0");
    case GL_SHADING_LANGUAGE_VERSION:
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "WebGL GLSL ES 1.0");
    case GL_VENDOR:
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "Bakery");
    case GL_RENDER:
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "WebGL 1.0 (OpenGL ES 2.0 Bakery Native OpenGL)");
    }
    _glSetError(GL_INVALID_ENUM);
    return v8::Null(v8::Isolate::GetCurrent());
}

#define CHECK_VALID(x) if(!x.isValid()){_glSetError(GL_INVALID_OPERATION);return;}
#define CHECK_VALID2(x,y) if(!x.isValid() || !y.isValid()){_glSetError(GL_INVALID_OPERATION);return;}
#define CHECK_VALID3(x,y,z) if(!x.isValid() || !y.isValid() || !z.isValid()){_glSetError(GL_INVALID_OPERATION);return;}

#define CHECK_VALID_RETURN(x,ret) if(!x.isValid()){_glSetError(GL_INVALID_OPERATION);return (ret);}
#define CHECK_VALID_RETURNNULL(x) CHECK_VALID_RETURN(x, v8::Null(v8::Isolate::GetCurrent()))

#define CHECK_LENGTH(x, len) if(x.size() < len){throw std::length_error(#x " must has at least " #len " elements");}

void _glUniformMatrix4fv(WebGLUniformLocation &location, bool transpose, const std::vector<GLfloat> &array)
{
    CHECK_VALID(location);
    CHECK_LENGTH(array, 16);
    glUniformMatrix4fv(location.location, array.size() / 16, transpose, array.data());
    CHECK_GL;
}

void _glUniformMatrix3fv(WebGLUniformLocation &location, bool transpose, const std::vector<GLfloat> &array)
{
    CHECK_VALID(location);
    CHECK_LENGTH(array, 9);
    glUniformMatrix3fv(location.location, array.size() / 9, transpose, array.data());
    CHECK_GL;
}

void _glUniformMatrix2fv(WebGLUniformLocation &location, bool transpose, const std::vector<GLfloat> &array)
{
    CHECK_VALID(location);
    CHECK_LENGTH(array, 4);
    glUniformMatrix2fv(location.location, array.size() / 4, transpose, array.data());
    CHECK_GL;
}

void _glUniform4iv(WebGLUniformLocation &location, const std::vector<GLint> &array)
{
    CHECK_VALID(location);
    CHECK_LENGTH(array, 4);
    glUniform4iv(location.location, array.size() / 4, array.data());
    CHECK_GL;
}

void _glUniform3iv(WebGLUniformLocation &location, const std::vector<GLint> &array)
{
    CHECK_VALID(location);
    CHECK_LENGTH(array, 3);
    glUniform4iv(location.location, array.size() / 3, array.data());
    CHECK_GL;
}

void _glUniform2iv(WebGLUniformLocation &location, const std::vector<GLint> &array)
{
    CHECK_VALID(location);
    CHECK_LENGTH(array, 2);
    glUniform4iv(location.location, array.size() / 2, array.data());
    CHECK_GL;
}

void _glUniform1iv(WebGLUniformLocation &location, const std::vector<GLint> &array)
{
    CHECK_VALID(location);
    CHECK_LENGTH(array, 1);
    glUniform4iv(location.location, array.size(), array.data());
    CHECK_GL;
}

void _glUniform4fv(WebGLUniformLocation &location, const std::vector<GLfloat> &array)
{
    CHECK_VALID(location);
    CHECK_LENGTH(array, 4);
    glUniform4fv(location.location, array.size() / 4, array.data());
    CHECK_GL;
}

void _glUniform3fv(WebGLUniformLocation &location, const std::vector<GLfloat> &array)
{
    CHECK_VALID(location);
    CHECK_LENGTH(array, 3);
    glUniform3fv(location.location, array.size() / 3, array.data());
    CHECK_GL;
}

void _glUniform2fv(WebGLUniformLocation &location, const std::vector<GLfloat> &array)
{
    CHECK_VALID(location);
    CHECK_LENGTH(array, 2);
    glUniform2fv(location.location, array.size() / 2, array.data());
    CHECK_GL;
}

void _glUniform1fv(WebGLUniformLocation &location, const std::vector<GLfloat> &array)
{
    CHECK_VALID(location);
    CHECK_LENGTH(array, 1);
    glUniform1fv(location.location, array.size(), array.data());
    CHECK_GL;
}

void _glUniform1i(WebGLUniformLocation &location, GLint v0)
{
    CHECK_VALID(location);
    glUniform1i(location.location, v0);
    CHECK_GL;
}

void _glUniform2i(WebGLUniformLocation &location, GLint v0, GLint v1)
{
    CHECK_VALID(location);
    glUniform2i(location.location, v0, v1);
    CHECK_GL;
}

void _glUniform3i(WebGLUniformLocation &location, GLint v0, GLint v1, GLint v2)
{
    CHECK_VALID(location);
    glUniform3i(location.location, v0, v1, v2);
    CHECK_GL;
}

void _glUniform4i(WebGLUniformLocation &location, GLint v0, GLint v1, GLint v2, GLint v3)
{
    CHECK_VALID(location);
    glUniform4i(location.location, v0, v1, v2, v3);
    CHECK_GL;
}

void _glUniform1f(WebGLUniformLocation &location, GLfloat v0)
{
    CHECK_VALID(location);
    glUniform1f(location.location, v0);
    CHECK_GL;
}

void _glUniform2f(WebGLUniformLocation &location, GLfloat v0, GLfloat v1)
{
    CHECK_VALID(location);
    glUniform2f(location.location, v0, v1);
    CHECK_GL;
}

void _glUniform3f(WebGLUniformLocation &location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    CHECK_VALID(location);
    glUniform3f(location.location, v0, v1, v2);
    CHECK_GL;
}

void _glUniform4f(WebGLUniformLocation &location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    CHECK_VALID(location);
    glUniform4f(location.location, v0, v1, v2, v3);
    CHECK_GL;
}

void _glDrawElements(GLenum mode, GLsizei count, GLenum type, intptr_t indices)
{
    glDrawElements(mode, count, type, (const void*)indices);
    CHECK_GL;
    setSwapBufferTag();
}

v8::Local<v8::Object> _glCreateTexture()
{
    auto texture = new WebGLTexture();
    glGenTextures(1, &texture->textureID);
    CHECK_GL;
    return v8pp::class_<WebGLTexture>::import_external(v8::Isolate::GetCurrent(), texture);
}

void _glDeleteTexture(WebGLTexture &texture)
{
    CHECK_VALID(texture);
    glDeleteTextures(1, &texture.textureID);
    CHECK_GL;
    texture.Invalidate();
}

void _glBindTexture(GLenum target, WebGLTexture* texture)
{
    if (!texture)
    {
        glBindTexture(target, 0);
    }
    else
    {
        CHECK_VALID((*texture));
        glBindTexture(target, texture->textureID);
    }
    CHECK_GL;
}

//for compressed version, we don't use Pixel Storage Parameters
void _glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, const std::vector<unsigned char> &pixels)
{
    glCompressedTexImage2D(target, level, internalformat, width, height, border, pixels.size(), pixels.data());
    CHECK_GL;
}

void _glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, const std::vector<unsigned char> &pixels)
{
    glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, pixels.size(), pixels.data());
    CHECK_GL;
}

v8::Local<v8::Value> _getTexParameter(GLenum target, GLenum pname)
{
    GLint res;
    glGetTexParameteriv(target, pname, &res);
    auto v = glGetError();
    if (v == GL_NO_ERROR)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), res);
    }
    else
    {
        _glSetError(v);
        return v8::Null(v8::Isolate::GetCurrent());
    }
}

bool _glIsTexture(WebGLTexture *texture)
{
    if (!texture)
        return false;
    CHECK_VALID_RETURN((*texture), false);
    return true;
}

void _glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, v8::Local<v8::Value> pixels)
{
    if (!v8pp::convert<std::vector<unsigned char>>::is_valid(v8::Isolate::GetCurrent(), pixels))
    {
        unsigned char *buffer = (unsigned char*)malloc(4 * width * height);
        memset(buffer, 0, 4 * width * height);
        glTexImage2D(target, level, internalformat, width, height, border, format, type, buffer);
        free(buffer);
    }
    else
    {
        //here, no check for internalformat and type of pixels array
        std::vector<unsigned char> buffer = v8pp::from_v8<std::vector<unsigned char>>(v8::Isolate::GetCurrent(), pixels);
        //TODO: check whether the size of the buffer is enough
        glTexImage2D(target, level, internalformat, width, height, border, format, type, buffer.data());
    }
    CHECK_GL;
}

v8::Local<v8::Object> _glCreateProgram()
{
    auto program = new WebGLProgram();
    program->program = glCreateProgram();
    CHECK_GL;
    return v8pp::class_<WebGLProgram>::import_external(v8::Isolate::GetCurrent(), program);
}

void _glDeleteProgram(WebGLProgram &program)
{
    CHECK_VALID(program);
    glDeleteProgram(program.program);
    CHECK_GL;
    program.Invalidate();
}

v8::Local<v8::Object> _glCreateShader(GLenum type)
{
    auto shader = new WebGLShader();
    shader->shader = glCreateShader(type);
    shader->type = type;
    CHECK_GL;
    return v8pp::class_<WebGLShader>::import_external(v8::Isolate::GetCurrent(), shader);
}

void _glDeleteShader(WebGLShader &shader)
{
    CHECK_VALID(shader);
    glDeleteShader(shader.shader);
    CHECK_GL;
    shader.Invalidate();
}

v8::Local<v8::Object> _glCreateRenderbuffer()
{
    auto rbo = new WebGLRenderbuffer();
    glGenRenderbuffers(1, &rbo->rbo);
    CHECK_GL;
    return v8pp::class_<WebGLRenderbuffer>::import_external(v8::Isolate::GetCurrent(), rbo);
}

void _glDeleteRenderbuffer(WebGLRenderbuffer &rbo)
{
    CHECK_VALID(rbo);
    glDeleteRenderbuffers(1, &rbo.rbo);
    CHECK_GL;
    rbo.Invalidate();
}

void _glBindRenderbuffer(GLenum target, WebGLRenderbuffer* rbo)
{
    if (!rbo)
    {
        glBindRenderbuffer(target, 0);
    }
    else
    {
        CHECK_VALID((*rbo));
        glBindRenderbuffer(target, rbo->rbo);
    }
    CHECK_GL;
}

v8::Local<v8::Value> _glGetRenderbufferParameter(GLenum target, GLenum pname)
{
    GLint res;
    glGetRenderbufferParameteriv(target, pname, &res);
    auto v = glGetError();
    if (v == GL_NO_ERROR)
    {
        return v8::Int32::New(v8::Isolate::GetCurrent(), res);
    }
    else
    {
        _glSetError(v);
        return v8::Null(v8::Isolate::GetCurrent());
    }
}

bool _glIsRenderbuffer(WebGLRenderbuffer* rbo)
{
    if (!rbo)
        return false;
    CHECK_VALID_RETURN((*rbo), false);
    return true;
}

v8::Local<v8::Object> _glCreateFramebuffer()
{
    auto fbo = new WebGLFramebuffer();
    glGenFramebuffers(1, &fbo->fbo);
    CHECK_GL;
    return v8pp::class_<WebGLFramebuffer>::import_external(v8::Isolate::GetCurrent(), fbo);
}

void _glDeleteFramebuffer(WebGLFramebuffer &fbo)
{
    CHECK_VALID(fbo);
    glDeleteFramebuffers(1, &fbo.fbo);
    CHECK_GL;
    fbo.Invalidate();
}

void _glBindFramebuffer(GLenum target, WebGLFramebuffer *fbo)
{
    if (!fbo)
    {
        glBindFramebuffer(target, 0);
    }
    else
    {
        CHECK_VALID((*fbo));
        glBindFramebuffer(target, fbo->fbo);
    }
    CHECK_GL;
}

void _glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, WebGLRenderbuffer& rbo)
{
    CHECK_VALID(rbo);
    glFramebufferRenderbuffer(target, attachment, renderbuffertarget, rbo.rbo);
    CHECK_GL;
}

void _glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, WebGLTexture& texture, GLint level)
{
    CHECK_VALID(texture);
    glFramebufferTexture2D(target, attachment, textarget, texture.textureID, level);
    CHECK_GL;
}

v8::Local<v8::Value> _glGetFramebufferAttachmentParameter(GLenum target, GLenum attachment, GLenum pname)
{
    switch (pname)
    {
    case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
    case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL:
    case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE:
        {
            GLint value;
            glGetFramebufferAttachmentParameteriv(target, attachment, pname, &value);
            auto v = glGetError();
            if (v == GL_NO_ERROR)
            {
                return v8::Int32::New(v8::Isolate::GetCurrent(), value);
            }
            else
            {
                _glSetError(v);
                return v8::Null(v8::Isolate::GetCurrent());
            }
        }
    case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
        {
            //query type first
            GLint value;
            glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &value);
            auto v = glGetError();
            if (v != GL_NO_ERROR)
            {
                _glSetError(v);
                return v8::Null(v8::Isolate::GetCurrent());
            }
            GLint res;
            glGetFramebufferAttachmentParameteriv(target, attachment, pname, &res);
            v = glGetError();
            if (v != GL_NO_ERROR)
            {
                _glSetError(v);
                return v8::Null(v8::Isolate::GetCurrent());
            }
            if (value == GL_NONE)
            {
                return v8::Null(v8::Isolate::GetCurrent());
            }
            else if (value == GL_RENDERBUFFER)
            {
                WebGLRenderbuffer *rbo = new WebGLRenderbuffer();
                rbo->rbo = (GLuint)res;
                return v8pp::class_<WebGLRenderbuffer>::import_external(v8::Isolate::GetCurrent(), rbo);
            }
            else if (value == GL_TEXTURE)
            {
                WebGLTexture *tex = new WebGLTexture();
                tex->textureID = (GLuint)res;
                return v8pp::class_<WebGLTexture>::import_external(v8::Isolate::GetCurrent(), tex);
            }
            else
            {
                return v8::Null(v8::Isolate::GetCurrent());
            }
        }
    }
    _glSetError(GL_INVALID_ENUM);
    return v8::Null(v8::Isolate::GetCurrent());
}

bool _glIsFramebuffer(WebGLFramebuffer *fbo)
{
    if (!fbo)
        return false;
    CHECK_VALID_RETURN((*fbo), false);
    return true;
}

v8::Local<v8::Object> _glCreateBuffer()
{
    auto buffer = new WebGLBuffer();
    glGenBuffers(1, &buffer->buffer);
    CHECK_GL;
    return v8pp::class_<WebGLBuffer>::import_external(v8::Isolate::GetCurrent(), buffer);
}

void _glDeleteBuffer(WebGLBuffer &buffer)
{
    CHECK_VALID(buffer);
    glDeleteBuffers(1, &buffer.buffer);
    CHECK_GL;
    buffer.Invalidate();
}

void _glShaderSource(WebGLShader &shader, const std::string &source)
{
    CHECK_VALID(shader);
#ifdef BK_ENABLE_SHADER_TRANSLATOR
    std::string s;
    bool success = BKShaderTranslator::translate(shader.type, source, s);
    if (!success) {
        s = mapShader(source.c_str(), shader.type);
    }
#else
    auto s = mapShader(source.c_str(), shader.type);
#endif
    auto v = s.c_str();
    glShaderSource(shader.shader, 1, &v, NULL);
    CHECK_GL;
}

void _glCompileShader(WebGLShader &shader)
{
    CHECK_VALID(shader);
    glCompileShader(shader.shader);
    CHECK_GL;
}

void _glAttachShader(WebGLProgram &program, WebGLShader &shader)
{
    CHECK_VALID2(program, shader);
    glAttachShader(program.program, shader.shader);
    CHECK_GL;
}

void _glDetachShader(WebGLProgram &program, WebGLShader &shader)
{
    CHECK_VALID2(program, shader);
    glDetachShader(program.program, shader.shader);
    CHECK_GL;
}

void _glLinkProgram(WebGLProgram &program)
{
    CHECK_VALID(program);
    glLinkProgram(program.program);
    CHECK_GL;
}

void _glUseProgram(WebGLProgram &program)
{
    CHECK_VALID(program);
    glUseProgram(program.program);
    CHECK_GL;
}

v8::Local<v8::Value> _glGetShaderParameter(WebGLShader &shader, GLenum pname)
{
    CHECK_VALID_RETURNNULL(shader);
    GLint res;
    switch (pname)
    {
    case GL_SHADER_TYPE:
        glGetShaderiv(shader.shader, pname, &res);
        CHECK_GL;
        return v8::Int32::New(v8::Isolate::GetCurrent(), res);
    case GL_DELETE_STATUS:
    case GL_COMPILE_STATUS:
        glGetShaderiv(shader.shader, pname, &res);
        CHECK_GL;
        return v8::Boolean::New(v8::Isolate::GetCurrent(), res);
    }
    _glSetError(GL_INVALID_ENUM);
    return v8::Null(v8::Isolate::GetCurrent());
}

v8::Local<v8::Value> _glGetShaderInfoLog(WebGLShader &shader)
{
    CHECK_VALID_RETURNNULL(shader);
    GLint len;
    glGetShaderiv(shader.shader, GL_INFO_LOG_LENGTH, &len);
    CHECK_GL;
    GLchar *buf = (GLchar*)malloc(len);
    glGetShaderInfoLog(shader.shader, len, &len, buf);
    CHECK_GL;
    auto res = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), buf);
    free(buf);
    return res;
}

v8::Local<v8::Value> _glGetProgramParameter(WebGLProgram &program, GLenum pname)
{
    CHECK_VALID_RETURNNULL(program);
    GLint res;
    switch (pname)
    {
    case GL_DELETE_STATUS:
    case GL_LINK_STATUS:
    case GL_VALIDATE_STATUS:
        glGetProgramiv(program.program, pname, &res);
        CHECK_GL;
        return v8::Boolean::New(v8::Isolate::GetCurrent(), res);
    case GL_ATTACHED_SHADERS:
    case GL_ACTIVE_ATTRIBUTES:
    case GL_ACTIVE_UNIFORMS:
        glGetProgramiv(program.program, pname, &res);
        CHECK_GL;
        return v8::Int32::New(v8::Isolate::GetCurrent(), res);
    }
    _glSetError(GL_INVALID_ENUM);
    return v8::Null(v8::Isolate::GetCurrent());
}

v8::Local<v8::Value> _glGetProgramInfoLog(WebGLProgram &program)
{
    CHECK_VALID_RETURNNULL(program);
    GLint len;
    glGetProgramiv(program.program, GL_INFO_LOG_LENGTH, &len);
    CHECK_GL;
    GLchar *buf = (GLchar*)malloc(len);
    glGetProgramInfoLog(program.program, len, &len, buf);
    CHECK_GL;
    auto res = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), buf);
    free(buf);
    return res;
}

void _glBufferData(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 3)
    {
        throw std::invalid_argument("need as least 3 parameters");
    }
    GLenum target = v8pp::from_v8<GLenum>(v8::Isolate::GetCurrent(), args[0]);
    GLenum usage = v8pp::from_v8<GLenum>(v8::Isolate::GetCurrent(), args[2]);
    if (args.Length() > 3)
    {
        //webGL2
        if(args.Length() < 5)
            throw std::invalid_argument("need as least 5 parameters");
        GLuint srcOffset = v8pp::from_v8<GLenum>(v8::Isolate::GetCurrent(), args[3], 0);
        GLuint length = v8pp::from_v8<GLenum>(v8::Isolate::GetCurrent(), args[4], 0);
        //let opengl check
        //if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER && target != GL_COPY_READ_BUFFER
        //	&& target != GL_COPY_WRITE_BUFFER && target != GL_TRANSFORM_FEEDBACK_BUFFER && target != GL_UNIFORM_BUFFER
        //	&& target != GL_PIXEL_PACK_BUFFER && target != GL_PIXEL_UNPACK_BUFFER)
        //{
        //	_glSetError(GL_INVALID_ENUM);
        //	return;
        //}
        //if (usage != GL_STATIC_DRAW && usage != GL_DYNAMIC_DRAW && usage != GL_STREAM_DRAW
        //	&& usage != GL_STATIC_READ && usage != GL_DYNAMIC_READ && usage != GL_STREAM_READ
        //	&& usage != GL_STATIC_COPY && usage != GL_DYNAMIC_COPY && usage != GL_STREAM_COPY)
        //{
        //	_glSetError(GL_INVALID_ENUM);
        //	return;
        //}
        if (!args[1].IsEmpty() && args[1]->IsArrayBuffer())
        {
            v8::Local<v8::ArrayBuffer> array = args[1].As<v8::ArrayBuffer>();
            auto content = array->GetContents();
            if (length <= 0 && content.ByteLength() > srcOffset)
                length = content.ByteLength() - srcOffset;
            glBufferData(target, length, (char*)content.Data() + srcOffset, usage);
            CHECK_GL;
            return;
        }
        if (!args[1].IsEmpty() && args[1]->IsArrayBufferView())
        {
            v8::Local<v8::ArrayBufferView> array = args[1].As<v8::ArrayBufferView>();
            if (array->HasBuffer())
            {
                auto content = array->Buffer()->GetContents();
                if (length <= 0 && content.ByteLength() > srcOffset)
                    length = content.ByteLength() - srcOffset;
                glBufferData(target, length, (char*)content.Data() + srcOffset, usage);
                CHECK_GL;
                return;
            }
            else
            {
                auto size = array->ByteLength();
                std::unique_ptr<char[]> data = std::make_unique<char[]>(size);
                array->CopyContents(data.get(), size);
                if (length <= 0 && size > srcOffset)
                    length = size - srcOffset;
                glBufferData(target, size, data.get(), usage);
                CHECK_GL;
                return;
            }
        }
        _glSetError(GL_INVALID_VALUE);
        return;
    }
    //let opengl check
    //if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER)
    //{
    //	_glSetError(GL_INVALID_ENUM);
    //	return;
    //}
    //if (usage != GL_STATIC_DRAW && usage != GL_DYNAMIC_DRAW && usage != GL_STREAM_DRAW)
    //{
    //	_glSetError(GL_INVALID_ENUM);
    //	return;
    //}
    if (!args[1].IsEmpty() && args[1]->IsNumber())
    {
        GLsizeiptr size = v8pp::from_v8<GLsizeiptr>(v8::Isolate::GetCurrent(), args[1]);
        glBufferData(target, size, NULL, usage);
        CHECK_GL;
        return;
    }
    if (!args[1].IsEmpty() && args[1]->IsArrayBuffer())
    {
        v8::Local<v8::ArrayBuffer> array = args[1].As<v8::ArrayBuffer>();
        auto content = array->GetContents();
        glBufferData(target, content.ByteLength(), content.Data(), usage);
        CHECK_GL;
        return;
    }
    if (!args[1].IsEmpty() && args[1]->IsArrayBufferView())
    {
        v8::Local<v8::ArrayBufferView> array = args[1].As<v8::ArrayBufferView>();
        if (array->HasBuffer())
        {
            auto content = array->Buffer()->GetContents();
            glBufferData(target, content.ByteLength(), content.Data(), usage);
            CHECK_GL;
            return;
        }
        else
        {
            auto size = array->ByteLength();
            std::unique_ptr<char[]> data = std::make_unique<char[]>(size);
            array->CopyContents(data.get(), size);
            glBufferData(target, size, data.get(), usage);
            CHECK_GL;
            return;
        }
    }
    _glSetError(GL_INVALID_VALUE);
}

void _glBufferSubData(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 3)
    {
        throw std::invalid_argument("need as least 3 parameters");
    }
    GLenum target = v8pp::from_v8<GLenum>(v8::Isolate::GetCurrent(), args[0]);
    GLintptr dstByteOffset = v8pp::from_v8<GLintptr>(v8::Isolate::GetCurrent(), args[1]);
    GLuint srcOffset = 0;
    GLuint length = 0;
    if (args.Length() > 3)
    {
        //webGL2
        if (args.Length() < 5)
            throw std::invalid_argument("need as least 5 parameters");
        GLuint srcOffset = v8pp::from_v8<GLenum>(v8::Isolate::GetCurrent(), args[3], 0);
        GLuint length = v8pp::from_v8<GLenum>(v8::Isolate::GetCurrent(), args[4], 0);
    }
    if (!args[2].IsEmpty() && args[2]->IsArrayBuffer())
    {
        v8::Local<v8::ArrayBuffer> array = args[2].As<v8::ArrayBuffer>();
        auto content = array->GetContents();
        if (length <= 0 && content.ByteLength() > srcOffset)
            length = content.ByteLength() - srcOffset;
        glBufferSubData(target, dstByteOffset, length, (char*)content.Data() + srcOffset);
        CHECK_GL;
        return;
    }
    if (!args[2].IsEmpty() && args[2]->IsArrayBufferView())
    {
        v8::Local<v8::ArrayBufferView> array = args[2].As<v8::ArrayBufferView>();
        if (array->HasBuffer())
        {
            auto content = array->Buffer()->GetContents();
            if (length <= 0 && content.ByteLength() > srcOffset)
                length = content.ByteLength() - srcOffset;
            glBufferSubData(target, dstByteOffset, length, (char*)content.Data() + srcOffset);
            CHECK_GL;
            return;
        }
        else
        {
            auto size = array->ByteLength();
            std::unique_ptr<char[]> data = std::make_unique<char[]>(size);
            array->CopyContents(data.get(), size);
            if (length <= 0 && size > srcOffset)
                length = size - srcOffset;
            glBufferSubData(target, dstByteOffset, length, (char*)data.get() + srcOffset);
            CHECK_GL;
            return;
        }
    }
    _glSetError(GL_INVALID_VALUE);
    return;
}

v8::Local<v8::Value> _glGetBufferParameter(GLenum target, GLenum pname)
{
    GLint params;
    glGetBufferParameteriv(target, pname, &params);
    auto err = glGetError();
    if (err != GL_NO_ERROR)
    {
        _glSetError(err);
        return v8::Null(v8::Isolate::GetCurrent());
    }
    return v8pp::to_v8(v8::Isolate::GetCurrent(), params);
}

bool _glIsBuffer(WebGLBuffer *buffer)
{
    if (buffer == NULL)
        return false;
    CHECK_VALID_RETURN((*buffer), false);
    return true;
}

void _glBindBuffer(GLenum target, WebGLBuffer &buffer)
{
    CHECK_VALID(buffer);
    glBindBuffer(target, buffer.buffer);
    CHECK_GL;
}

GLint _glGetAttribLocation(WebGLProgram &program, const std::string name)
{
    CHECK_VALID_RETURN(program, -1);
    GLint res = glGetAttribLocation(program.program, (variable_prefix + name).c_str());
    CHECK_GL;
    return res;
}

v8::Local<v8::Value> _glGetUniformLocation(WebGLProgram &program, const std::string name)
{
    CHECK_VALID_RETURNNULL(program);
    if (name.find("webgl_") == 0 || name.find("_webgl_") == 0)
    {
        return v8::Null(v8::Isolate::GetCurrent());
    }
    if (name.length() > 256)
    {
        _glSetError(GL_INVALID_VALUE);
        return v8::Null(v8::Isolate::GetCurrent());
    }
    GLint loc = glGetUniformLocation(program.program, (variable_prefix + name).c_str());
    CHECK_GL;
    if (loc == -1)
    {
        return v8::Null(v8::Isolate::GetCurrent());
    }
    auto locobj = new WebGLUniformLocation();
    locobj->location = loc;
    return v8pp::class_<WebGLUniformLocation>::import_external(v8::Isolate::GetCurrent(), locobj);
}

void _glClear(GLbitfield bits)
{
    glClear(bits);
    CHECK_GL;
    setSwapBufferTag();
}

void _glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    glDrawArrays(mode, first, count);
    CHECK_GL;
    setSwapBufferTag();
}

void _glFinish()
{
    glFinish();
    CHECK_GL;
    setSwapBufferTag();
}

void _glFlush()
{
    glFlush();
    CHECK_GL;
    setSwapBufferTag();
}

#define BIND_CLASS(x) v8pp::class_<x> class_##x(iso);class_##x.inherit<WebGLObject>();global->Set(v8pp::to_v8(iso, #x), class_##x.js_function_template()->GetFunction());

#define BIND_CLASS_WITH_MEMBER(x, member) v8pp::class_<x> class_##x(iso);\
    class_##x.inherit<WebGLObject>();\
    class_##x member;\
global->Set(v8pp::to_v8(iso, #x), class_##x.js_function_template()->GetFunction());

#define ADD_CONST(name) .set_const(#name, GL_##name)

//directly map function to native openGL function
void Bind_GL(v8::Isolate * iso)
{
    auto global = iso->GetCurrentContext()->Global();

    v8pp::class_<WebGLObject> class_WebGLObject(iso);
    global->Set(v8pp::to_v8(iso, "WebGLObject"), class_WebGLObject.create_object(iso));

    BIND_CLASS_WITH_MEMBER(WebGLActiveInfo,
        .set("size", v8pp::property(&WebGLActiveInfo::getSize))
        .set("type", v8pp::property(&WebGLActiveInfo::getType))
        .set("name", v8pp::property(&WebGLActiveInfo::getName))
    );

    //WebGLBuffer
    BIND_CLASS(WebGLBuffer);

    //WebGLFramebuffer
    BIND_CLASS(WebGLFramebuffer);

    //WebGLProgram
    BIND_CLASS(WebGLProgram);

    //WebGLRenderbuffer
    BIND_CLASS(WebGLRenderbuffer);

    //WebGLShader
    BIND_CLASS(WebGLShader);

    //WebGLTexture
    BIND_CLASS(WebGLTexture);

    //WebGLUniformLocation
    BIND_CLASS(WebGLUniformLocation);

    //WebGLShaderPrecisionFormat
    BIND_CLASS_WITH_MEMBER(WebGLShaderPrecisionFormat,
        .set("rangeMin", v8pp::property(&WebGLShaderPrecisionFormat::getRangeMin))
        .set("rangeMax", v8pp::property(&WebGLShaderPrecisionFormat::getRangeMax))
        .set("precision", v8pp::property(&WebGLShaderPrecisionFormat::getPrecision))
    );
}

v8::Local<v8::Object> getGLmodule()
{
    static v8pp::module module_GL(v8::Isolate::GetCurrent());
    static bool module_init = false;
    if (!module_init)
    {
        module_GL
            //Getting information about the context
            .set("getContextAttributes", _glGetContextAttributes)
            //Setting and getting state
            .set("activeTexture", glActiveTexture)
            .set("blendColor", glBlendColor)
            .set("blendEquation", glBlendEquation)
            .set("blendEquationSeparate", glBlendEquationSeparate)
            .set("blendFunc", glBlendFunc)
            .set("blendFuncSeparate", glBlendFuncSeparate)
            .set("clearColor", glClearColor)
            .set("clearDepth", glClearDepth)
            .set("clearStencil", glClearStencil)
            .set("colorMask", glColorMask)
            .set("cullFace", glCullFace)
            .set("depthFunc", glDepthFunc)
            .set("depthMask", glDepthMask)
            .set("depthRange", glDepthRange)
            .set("disable", glDisable)
            .set("enable", glEnable)
            .set("frontFace", glFrontFace)
            .set("getParameter", _glGetParameter)
            .set("getError", _glGetError)
            .set("hint", glHint)
            .set("isEnabled", glIsEnabled)
            .set("lineWidth", glLineWidth)
            .set("pixelStorei", glPixelStorei)
            .set("polygonOffset", glPolygonOffset)
            .set("sampleCoverage", glSampleCoverage)
            .set("stencilFunc", glStencilFunc)
            .set("stencilFuncSeparate", glStencilFuncSeparate)
            .set("stencilMask", glStencilMask)
            .set("stencilMaskSeparate", glStencilMaskSeparate)
            .set("stencilOp", glStencilOp)
            .set("stencilOpSeparate", glStencilOpSeparate)
            //Viewing and clipping
            .set("scissor", glScissor)
            .set("viewport", glViewport)
            //Buffer objects
            .set("bindBuffer", _glBindBuffer)
            .set("bufferData", _glBufferData)
            .set("bufferSubData", _glBufferSubData)
            .set("createBuffer", _glCreateBuffer)
            .set("deleteBuffer", _glDeleteBuffer)
            .set("getBufferParameter", _glGetBufferParameter)
            .set("isBuffer", _glIsBuffer)
            //Framebuffer objects
            .set("bindFramebuffer", _glBindFramebuffer)
            .set("checkFramebufferStatus", glCheckFramebufferStatus)
            .set("createFramebuffer", _glCreateFramebuffer)
            .set("deleteFramebuffer", _glDeleteFramebuffer)
            .set("framebufferRenderbuffer", _glFramebufferRenderbuffer)
            .set("framebufferTexture2D", _glFramebufferTexture2D)
            .set("getFramebufferAttachmentParameter", _glGetFramebufferAttachmentParameter)
            .set("isFramebuffer", _glIsFramebuffer)
            //Renderbuffer objects
            .set("bindRenderbuffer", _glBindRenderbuffer)
            .set("createRenderbuffer", _glCreateRenderbuffer)
            .set("deleteRenderbuffer", _glDeleteRenderbuffer)
            .set("getRenderbufferParameter", _glGetRenderbufferParameter)
            .set("isRenderbuffer", _glIsRenderbuffer)
            .set("renderbufferStorage", glRenderbufferStorage)
            //Texture objects
            .set("bindTexture", _glBindTexture)
            .set("compressedTexImage2D", _glCompressedTexImage2D)
            .set("compressedTexSubImage2D", _glCompressedTexSubImage2D)
            .set("copyTexImage2D", glCopyTexImage2D)
            .set("copyTexSubImage2D", glCopyTexSubImage2D)
            .set("createTexture", _glCreateTexture)
            .set("deleteTexture", _glDeleteTexture)
            .set("generateMipmap", glGenerateMipmap)
            .set("getTexParameter", _getTexParameter)
            .set("isTexture", _glIsTexture)
            //.set("texImage2D", _glTexImage2D)
            .set("texParameterf", glTexParameterf)
            .set("texParameteri", glTexParameteri)
            //.set("texSubImage2D", _glTexSubImage2D)
            //Programs and Shaders
            .set("attachShader", _glAttachShader)
            .set("bindAttribLocation", glBindAttribLocation)
            .set("compileShader", _glCompileShader)
            .set("createProgram", _glCreateProgram)
            .set("createShader", _glCreateShader)
            .set("deleteProgram", _glDeleteProgram)
            .set("deleteShader", _glDeleteShader)
            .set("detachShader", _glDetachShader)
            //.set("getAttachedShaders", _glGetAttachedShaders)
            .set("getProgramParameter", _glGetProgramParameter)
            .set("getProgramInfoLog", _glGetProgramInfoLog)
            .set("getShaderParameter", _glGetShaderParameter)
            //.set("getShaderPrecisionFormat", _glGetShaderPrecisionFormat)
            .set("getShaderInfoLog", _glGetShaderInfoLog)
            //.set("getShaderSource", _glGetShaderSource)
            .set("isProgram", glIsProgram)
            .set("isShader", glIsShader)
            .set("linkProgram", _glLinkProgram)
            .set("shaderSource", _glShaderSource)
            .set("useProgram", _glUseProgram)
            //.set("validateProgram", _glValidateProgram)
            //Uniforms and attributes
            .set("disableVertexAttribArray", glDisableVertexAttribArray)
            .set("enableVertexAttribArray", glEnableVertexAttribArray)
            //.set("getActiveAttrib", _glGetActiveAttrib)
            //.set("getActiveUniform", _glGetActiveUniform)
            .set("getAttribLocation", _glGetAttribLocation)
            //.set("getUniform", _glGetUniform)
            .set("getUniformLocation", _glGetUniformLocation)
            //.set("getVertexAttrib", _glGetVertexAttrib)
            //.set("getVertexAttribOffset", _glGetVertexAttribOffset)
            .set("uniform1f", _glUniform1f)
            .set("uniform1i", _glUniform1i)
            .set("uniform2f", _glUniform2f)
            .set("uniform2i", _glUniform2i)
            .set("uniform3f", _glUniform3f)
            .set("uniform3i", _glUniform3i)
            .set("uniform4f", _glUniform4f)
            .set("uniform4i", _glUniform4i)
            .set("uniform1fv", _glUniform1fv)
            .set("uniform1iv", _glUniform1iv)
            .set("uniform2fv", _glUniform2fv)
            .set("uniform2iv", _glUniform2iv)
            .set("uniform3fv", _glUniform3fv)
            .set("uniform3iv", _glUniform3iv)
            .set("uniform4fv", _glUniform4fv)
            .set("uniform4iv", _glUniform4iv)
            .set("uniformMatrix2fv", _glUniformMatrix2fv)
            .set("uniformMatrix3fv", _glUniformMatrix3fv)
            .set("uniformMatrix4fv", _glUniformMatrix4fv)
            .set("vertexAttrib1f", glVertexAttrib1f)
            .set("vertexAttrib2f", glVertexAttrib2f)
            .set("vertexAttrib3f", glVertexAttrib3f)
            .set("vertexAttrib4f", glVertexAttrib4f)
            //.set("vertexAttrib1fv", _glVertexAttrib1fv)
            //.set("vertexAttrib2fv", _glVertexAttrib2fv)
            //.set("vertexAttrib3fv", _glVertexAttrib3fv)
            //.set("vertexAttrib4fv", _glVertexAttrib4fv)
            .set("vertexAttribPointer", _glVertexAttribPointer)
            //Writing to the drawing buffer
            .set("clear", _glClear)
            .set("drawArrays", _glDrawArrays)
            .set("drawElements", _glDrawElements)
            .set("finish", _glFinish)
            .set("flush", _glFlush)
            //Reading back pixels
            //.set("readPixels", _glReadPixels)
            //Detecting context lost events
            .set("isContextLost", _glIsContextLost)
            //Detecting and enabling extensions
            //.set("getSupportedExtensions", _glGetSupportedExtensions)
            //.set("getExtension", _glGetExtension)
            /* ClearBufferMask */
            ADD_CONST(DEPTH_BUFFER_BIT)
            ADD_CONST(STENCIL_BUFFER_BIT)
            ADD_CONST(COLOR_BUFFER_BIT)
            /* BeginMode */
            ADD_CONST(POINTS)
            ADD_CONST(LINES)
            ADD_CONST(LINE_LOOP)
            ADD_CONST(LINE_STRIP)
            ADD_CONST(TRIANGLES)
            ADD_CONST(TRIANGLE_STRIP)
            ADD_CONST(TRIANGLE_FAN)
            /* AlphaFunction (not supported in ES20) */
            /*      NEVER */
            /*      LESS */
            /*      EQUAL */
            /*      LEQUAL */
            /*      GREATER */
            /*      NOTEQUAL */
            /*      GEQUAL */
            /*      ALWAYS */
            /* BlendingFactorDest */
            ADD_CONST(ZERO)
            ADD_CONST(ONE)
            ADD_CONST(SRC_COLOR)
            ADD_CONST(ONE_MINUS_SRC_COLOR)
            ADD_CONST(SRC_ALPHA)
            ADD_CONST(ONE_MINUS_SRC_ALPHA)
            ADD_CONST(DST_ALPHA)
            ADD_CONST(ONE_MINUS_DST_ALPHA)
            /* BlendingFactorSrc */
            /*      ZERO */
            /*      ONE */
            ADD_CONST(DST_COLOR)
            ADD_CONST(ONE_MINUS_DST_COLOR)
            ADD_CONST(SRC_ALPHA_SATURATE)
            /*      SRC_ALPHA */
            /*      ONE_MINUS_SRC_ALPHA */
            /*      DST_ALPHA */
            /*      ONE_MINUS_DST_ALPHA */

            /* BlendEquationSeparate */
            ADD_CONST(FUNC_ADD)
            ADD_CONST(BLEND_EQUATION)
            ADD_CONST(BLEND_EQUATION_RGB)
            ADD_CONST(BLEND_EQUATION_ALPHA)

            /* BlendSubtract */
            ADD_CONST(FUNC_SUBTRACT)
            ADD_CONST(FUNC_REVERSE_SUBTRACT)

            /* Separate Blend Functions */
            ADD_CONST(BLEND_DST_RGB)
            ADD_CONST(BLEND_SRC_RGB)
            ADD_CONST(BLEND_DST_ALPHA)
            ADD_CONST(BLEND_SRC_ALPHA)
            ADD_CONST(CONSTANT_COLOR)
            ADD_CONST(ONE_MINUS_CONSTANT_COLOR)
            ADD_CONST(CONSTANT_ALPHA)
            ADD_CONST(ONE_MINUS_CONSTANT_ALPHA)
            ADD_CONST(BLEND_COLOR)

            /* Buffer Objects */
            ADD_CONST(ARRAY_BUFFER)
            ADD_CONST(ELEMENT_ARRAY_BUFFER)
            ADD_CONST(ARRAY_BUFFER_BINDING)
            ADD_CONST(ELEMENT_ARRAY_BUFFER_BINDING)

            ADD_CONST(STREAM_DRAW)
            ADD_CONST(STATIC_DRAW)
            ADD_CONST(DYNAMIC_DRAW)

            ADD_CONST(BUFFER_SIZE)
            ADD_CONST(BUFFER_USAGE)

            ADD_CONST(CURRENT_VERTEX_ATTRIB)

            /* CullFaceMode */
            ADD_CONST(FRONT)
            ADD_CONST(BACK)
            ADD_CONST(FRONT_AND_BACK)

            /* DepthFunction */
            /*      NEVER */
            /*      LESS */
            /*      EQUAL */
            /*      LEQUAL */
            /*      GREATER */
            /*      NOTEQUAL */
            /*      GEQUAL */
            /*      ALWAYS */

            /* EnableCap */
            /* TEXTURE_2D */
            ADD_CONST(CULL_FACE)
            ADD_CONST(BLEND)
            ADD_CONST(DITHER)
            ADD_CONST(STENCIL_TEST)
            ADD_CONST(DEPTH_TEST)
            ADD_CONST(SCISSOR_TEST)
            ADD_CONST(POLYGON_OFFSET_FILL)
            ADD_CONST(SAMPLE_ALPHA_TO_COVERAGE)
            ADD_CONST(SAMPLE_COVERAGE)

            /* ErrorCode */
            ADD_CONST(NO_ERROR)
            ADD_CONST(INVALID_ENUM)
            ADD_CONST(INVALID_VALUE)
            ADD_CONST(INVALID_OPERATION)
            ADD_CONST(OUT_OF_MEMORY)

            /* FrontFaceDirection */
            ADD_CONST(CW)
            ADD_CONST(CCW)

            /* GetPName */
            ADD_CONST(LINE_WIDTH)
            ADD_CONST(ALIASED_POINT_SIZE_RANGE)
            ADD_CONST(ALIASED_LINE_WIDTH_RANGE)
            ADD_CONST(CULL_FACE_MODE)
            ADD_CONST(FRONT_FACE)
            ADD_CONST(DEPTH_RANGE)
            ADD_CONST(DEPTH_WRITEMASK)
            ADD_CONST(DEPTH_CLEAR_VALUE)
            ADD_CONST(DEPTH_FUNC)
            ADD_CONST(STENCIL_CLEAR_VALUE)
            ADD_CONST(STENCIL_FUNC)
            ADD_CONST(STENCIL_FAIL)
            ADD_CONST(STENCIL_PASS_DEPTH_FAIL)
            ADD_CONST(STENCIL_PASS_DEPTH_PASS)
            ADD_CONST(STENCIL_REF)
            ADD_CONST(STENCIL_VALUE_MASK)
            ADD_CONST(STENCIL_WRITEMASK)
            ADD_CONST(STENCIL_BACK_FUNC)
            ADD_CONST(STENCIL_BACK_FAIL)
            ADD_CONST(STENCIL_BACK_PASS_DEPTH_FAIL)
            ADD_CONST(STENCIL_BACK_PASS_DEPTH_PASS)
            ADD_CONST(STENCIL_BACK_REF)
            ADD_CONST(STENCIL_BACK_VALUE_MASK)
            ADD_CONST(STENCIL_BACK_WRITEMASK)
            ADD_CONST(VIEWPORT)
            ADD_CONST(SCISSOR_BOX)
            /*      SCISSOR_TEST */
            ADD_CONST(COLOR_CLEAR_VALUE)
            ADD_CONST(COLOR_WRITEMASK)
            ADD_CONST(UNPACK_ALIGNMENT)
            ADD_CONST(PACK_ALIGNMENT)
            ADD_CONST(MAX_TEXTURE_SIZE)
            ADD_CONST(MAX_VIEWPORT_DIMS)
            ADD_CONST(SUBPIXEL_BITS)
            ADD_CONST(RED_BITS)
            ADD_CONST(GREEN_BITS)
            ADD_CONST(BLUE_BITS)
            ADD_CONST(ALPHA_BITS)
            ADD_CONST(DEPTH_BITS)
            ADD_CONST(STENCIL_BITS)
            ADD_CONST(POLYGON_OFFSET_UNITS)
            /*      POLYGON_OFFSET_FILL */
            ADD_CONST(POLYGON_OFFSET_FACTOR)
            ADD_CONST(TEXTURE_BINDING_2D)
            ADD_CONST(SAMPLE_BUFFERS)
            ADD_CONST(SAMPLES)
            ADD_CONST(SAMPLE_COVERAGE_VALUE)
            ADD_CONST(SAMPLE_COVERAGE_INVERT)

            /* GetTextureParameter */
            /*      TEXTURE_MAG_FILTER */
            /*      TEXTURE_MIN_FILTER */
            /*      TEXTURE_WRAP_S */
            /*      TEXTURE_WRAP_T */

            ADD_CONST(COMPRESSED_TEXTURE_FORMATS)

            /* HintMode */
            ADD_CONST(DONT_CARE)
            ADD_CONST(FASTEST)
            ADD_CONST(NICEST)

            /* HintTarget */
            ADD_CONST(GENERATE_MIPMAP_HINT)

            /* DataType */
            ADD_CONST(BYTE)
            ADD_CONST(UNSIGNED_BYTE)
            ADD_CONST(SHORT)
            ADD_CONST(UNSIGNED_SHORT)
            ADD_CONST(INT)
            ADD_CONST(UNSIGNED_INT)
            ADD_CONST(FLOAT)

            /* PixelFormat */
            ADD_CONST(DEPTH_COMPONENT)
            ADD_CONST(ALPHA)
            ADD_CONST(RGB)
            ADD_CONST(RGBA)
            ADD_CONST(LUMINANCE)
            ADD_CONST(LUMINANCE_ALPHA)

            /* PixelType */
            /*      UNSIGNED_BYTE */
            ADD_CONST(UNSIGNED_SHORT_4_4_4_4)
            ADD_CONST(UNSIGNED_SHORT_5_5_5_1)
            ADD_CONST(UNSIGNED_SHORT_5_6_5)

            /* Shaders */
            ADD_CONST(FRAGMENT_SHADER)
            ADD_CONST(VERTEX_SHADER)
            ADD_CONST(MAX_VERTEX_ATTRIBS)
            ADD_CONST(MAX_VERTEX_UNIFORM_VECTORS)
            ADD_CONST(MAX_VARYING_VECTORS)
            ADD_CONST(MAX_COMBINED_TEXTURE_IMAGE_UNITS)
            ADD_CONST(MAX_VERTEX_TEXTURE_IMAGE_UNITS)
            ADD_CONST(MAX_TEXTURE_IMAGE_UNITS)
            ADD_CONST(MAX_FRAGMENT_UNIFORM_VECTORS)
            ADD_CONST(SHADER_TYPE)
            ADD_CONST(DELETE_STATUS)
            ADD_CONST(LINK_STATUS)
            ADD_CONST(VALIDATE_STATUS)
            ADD_CONST(ATTACHED_SHADERS)
            ADD_CONST(ACTIVE_UNIFORMS)
            ADD_CONST(ACTIVE_ATTRIBUTES)
            ADD_CONST(SHADING_LANGUAGE_VERSION)
            ADD_CONST(CURRENT_PROGRAM)

            /* StencilFunction */
            ADD_CONST(NEVER)
            ADD_CONST(LESS)
            ADD_CONST(EQUAL)
            ADD_CONST(LEQUAL)
            ADD_CONST(GREATER)
            ADD_CONST(NOTEQUAL)
            ADD_CONST(GEQUAL)
            ADD_CONST(ALWAYS)

            /* StencilOp */
            /*      ZERO */
            ADD_CONST(KEEP)
            ADD_CONST(REPLACE)
            ADD_CONST(INCR)
            ADD_CONST(DECR)
            ADD_CONST(INVERT)
            ADD_CONST(INCR_WRAP)
            ADD_CONST(DECR_WRAP)

            /* StringName */
            ADD_CONST(VENDOR)
            ADD_CONST(RENDERER)
            ADD_CONST(VERSION)

            /* TextureMagFilter */
            ADD_CONST(NEAREST)
            ADD_CONST(LINEAR)

            /* TextureMinFilter */
            /*      NEAREST */
            /*      LINEAR */
            ADD_CONST(NEAREST_MIPMAP_NEAREST)
            ADD_CONST(LINEAR_MIPMAP_NEAREST)
            ADD_CONST(NEAREST_MIPMAP_LINEAR)
            ADD_CONST(LINEAR_MIPMAP_LINEAR)

            /* TextureParameterName */
            ADD_CONST(TEXTURE_MAG_FILTER)
            ADD_CONST(TEXTURE_MIN_FILTER)
            ADD_CONST(TEXTURE_WRAP_S)
            ADD_CONST(TEXTURE_WRAP_T)

            /* TextureTarget */
            ADD_CONST(TEXTURE_2D)
            ADD_CONST(TEXTURE)

            ADD_CONST(TEXTURE_CUBE_MAP)
            ADD_CONST(TEXTURE_BINDING_CUBE_MAP)
            ADD_CONST(TEXTURE_CUBE_MAP_POSITIVE_X)
            ADD_CONST(TEXTURE_CUBE_MAP_NEGATIVE_X)
            ADD_CONST(TEXTURE_CUBE_MAP_POSITIVE_Y)
            ADD_CONST(TEXTURE_CUBE_MAP_NEGATIVE_Y)
            ADD_CONST(TEXTURE_CUBE_MAP_POSITIVE_Z)
            ADD_CONST(TEXTURE_CUBE_MAP_NEGATIVE_Z)
            ADD_CONST(MAX_CUBE_MAP_TEXTURE_SIZE)

            /* TextureUnit */
            ADD_CONST(TEXTURE0)
            ADD_CONST(TEXTURE1)
            ADD_CONST(TEXTURE2)
            ADD_CONST(TEXTURE3)
            ADD_CONST(TEXTURE4)
            ADD_CONST(TEXTURE5)
            ADD_CONST(TEXTURE6)
            ADD_CONST(TEXTURE7)
            ADD_CONST(TEXTURE8)
            ADD_CONST(TEXTURE9)
            ADD_CONST(TEXTURE10)
            ADD_CONST(TEXTURE11)
            ADD_CONST(TEXTURE12)
            ADD_CONST(TEXTURE13)
            ADD_CONST(TEXTURE14)
            ADD_CONST(TEXTURE15)
            ADD_CONST(TEXTURE16)
            ADD_CONST(TEXTURE17)
            ADD_CONST(TEXTURE18)
            ADD_CONST(TEXTURE19)
            ADD_CONST(TEXTURE20)
            ADD_CONST(TEXTURE21)
            ADD_CONST(TEXTURE22)
            ADD_CONST(TEXTURE23)
            ADD_CONST(TEXTURE24)
            ADD_CONST(TEXTURE25)
            ADD_CONST(TEXTURE26)
            ADD_CONST(TEXTURE27)
            ADD_CONST(TEXTURE28)
            ADD_CONST(TEXTURE29)
            ADD_CONST(TEXTURE30)
            ADD_CONST(TEXTURE31)
            ADD_CONST(ACTIVE_TEXTURE)

            /* TextureWrapMode */
            ADD_CONST(REPEAT)
            ADD_CONST(CLAMP_TO_EDGE)
            ADD_CONST(MIRRORED_REPEAT)

            /* Uniform Types */
            ADD_CONST(FLOAT_VEC2)
            ADD_CONST(FLOAT_VEC3)
            ADD_CONST(FLOAT_VEC4)
            ADD_CONST(INT_VEC2)
            ADD_CONST(INT_VEC3)
            ADD_CONST(INT_VEC4)
            ADD_CONST(BOOL)
            ADD_CONST(BOOL_VEC2)
            ADD_CONST(BOOL_VEC3)
            ADD_CONST(BOOL_VEC4)
            ADD_CONST(FLOAT_MAT2)
            ADD_CONST(FLOAT_MAT3)
            ADD_CONST(FLOAT_MAT4)
            ADD_CONST(SAMPLER_2D)
            ADD_CONST(SAMPLER_CUBE)

            /* Vertex Arrays */
            ADD_CONST(VERTEX_ATTRIB_ARRAY_ENABLED)
            ADD_CONST(VERTEX_ATTRIB_ARRAY_SIZE)
            ADD_CONST(VERTEX_ATTRIB_ARRAY_STRIDE)
            ADD_CONST(VERTEX_ATTRIB_ARRAY_TYPE)
            ADD_CONST(VERTEX_ATTRIB_ARRAY_NORMALIZED)
            ADD_CONST(VERTEX_ATTRIB_ARRAY_POINTER)
            ADD_CONST(VERTEX_ATTRIB_ARRAY_BUFFER_BINDING)

            /* Read Format */
            ADD_CONST(IMPLEMENTATION_COLOR_READ_TYPE)
            ADD_CONST(IMPLEMENTATION_COLOR_READ_FORMAT)

            /* Shader Source */
            ADD_CONST(COMPILE_STATUS)

            /* Shader Precision-Specified Types */
            ADD_CONST(LOW_FLOAT)
            ADD_CONST(MEDIUM_FLOAT)
            ADD_CONST(HIGH_FLOAT)
            ADD_CONST(LOW_INT)
            ADD_CONST(MEDIUM_INT)
            ADD_CONST(HIGH_INT)

            /* Framebuffer Object. */
            ADD_CONST(FRAMEBUFFER)
            ADD_CONST(RENDERBUFFER)

            ADD_CONST(RGBA4)
            ADD_CONST(RGB5_A1)
            ADD_CONST(RGB565)
            ADD_CONST(DEPTH_COMPONENT16)
            ADD_CONST(STENCIL_INDEX8)
            ADD_CONST(DEPTH_STENCIL)

            ADD_CONST(RENDERBUFFER_WIDTH)
            ADD_CONST(RENDERBUFFER_HEIGHT)
            ADD_CONST(RENDERBUFFER_INTERNAL_FORMAT)
            ADD_CONST(RENDERBUFFER_RED_SIZE)
            ADD_CONST(RENDERBUFFER_GREEN_SIZE)
            ADD_CONST(RENDERBUFFER_BLUE_SIZE)
            ADD_CONST(RENDERBUFFER_ALPHA_SIZE)
            ADD_CONST(RENDERBUFFER_DEPTH_SIZE)
            ADD_CONST(RENDERBUFFER_STENCIL_SIZE)

            ADD_CONST(FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE)
            ADD_CONST(FRAMEBUFFER_ATTACHMENT_OBJECT_NAME)
            ADD_CONST(FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL)
            ADD_CONST(FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE)

            ADD_CONST(COLOR_ATTACHMENT0)
            ADD_CONST(DEPTH_ATTACHMENT)
            ADD_CONST(STENCIL_ATTACHMENT)
            ADD_CONST(DEPTH_STENCIL_ATTACHMENT)

            ADD_CONST(NONE)

            ADD_CONST(FRAMEBUFFER_COMPLETE)
            ADD_CONST(FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
            ADD_CONST(FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
            ADD_CONST(FRAMEBUFFER_INCOMPLETE_DIMENSIONS)
            ADD_CONST(FRAMEBUFFER_UNSUPPORTED)

            ADD_CONST(FRAMEBUFFER_BINDING)
            ADD_CONST(RENDERBUFFER_BINDING)
            ADD_CONST(MAX_RENDERBUFFER_SIZE)

            ADD_CONST(INVALID_FRAMEBUFFER_OPERATION)

            /* WebGL-specific enums */
            ADD_CONST(UNPACK_FLIP_Y_WEBGL)
            ADD_CONST(UNPACK_PREMULTIPLY_ALPHA_WEBGL)
            ADD_CONST(CONTEXT_LOST_WEBGL)
            ADD_CONST(UNPACK_COLORSPACE_CONVERSION_WEBGL)
            ADD_CONST(BROWSER_DEFAULT_WEBGL)
            ;
        module_init = true;
    }
    return module_GL.new_instance();
}

std::string mapShader(const char *src, GLenum shaderType)
{
    //https://www.khronos.org/opengl/wiki/Type_Qualifier_(GLSL), precision has no effect on desktop openGL
    static std::regex reg_precision("\\b(lowp|highp|mediump)\\b", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    std::string res = std::regex_replace(src, reg_precision, "", std::regex_constants::match_any);
    //remove .f or \digit f
    static std::regex reg_f("([0-9]\\w*\\.|[0-9])\\w*f", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    res = std::regex_replace(res, reg_f, "$1", std::regex_constants::match_any);
    //attribute => in
    static std::regex reg_attr("\\b(attribute)\\b", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    res = std::regex_replace(res, reg_attr, "in", std::regex_constants::match_any);
    //varying => in|out
    static std::regex reg_varying("\\b(varying)\\b", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    if (shaderType == GL_VERTEX_SHADER)
    {
        res = std::regex_replace(res, reg_varying, "out", std::regex_constants::match_any);
    }
    else
    {
        res = std::regex_replace(res, reg_varying, "in", std::regex_constants::match_any);
    }
    //gl_FragColor=>webgl_FragColor
    static std::regex reg_fragcolor("\\bgl_FragColor\\b", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    if (shaderType == GL_FRAGMENT_SHADER && std::regex_search(res, reg_fragcolor, std::regex_constants::match_any))
    {
        res = std::regex_replace(res, reg_fragcolor, "webgl_FragColor", std::regex_constants::match_any);
        res = "out vec4 webgl_FragColor;\n" + res;
    }
    //add #extension GL_ARB_gpu_shader5
    res = "#extension GL_ARB_gpu_shader5 : enable\n" + res;
    //add version 330
    res = "#version 330 core\n" + res;
    return res;
}

bool shouldSwapBuffer()
{
    return GL_shouldSwapBuffer;
}

void clearSwapBufferTag()
{
    GL_shouldSwapBuffer = false;
}

void setSwapBufferTag()
{
    GL_shouldSwapBuffer = true;
}

WebGLContextAttributes * getCurrentContextAttributes()
{
    return &GL_context;
}
