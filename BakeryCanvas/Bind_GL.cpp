#include "Bind_GL.h"

static GLenum inner_GLError = GL_NO_ERROR;
static GLboolean use_InnerError = false;

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

void _glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, intptr_t pointer)
{
	glVertexAttribPointer(index, size, type, normalized, stride, (const void*)pointer);
}

void _glVertexAttrib2fv(GLuint index, intptr_t v)
{
	glVertexAttrib2fv(index, (const GLfloat *)v);
}

void _glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, intptr_t value)
{
	glUniformMatrix4fv(location, count, transpose, (const GLfloat *)value);
}

void _glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, intptr_t value)
{
	glUniformMatrix3fv(location, count, transpose, (const GLfloat *)value);
}

void _glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, intptr_t value)
{
	glUniformMatrix2fv(location, count, transpose, (const GLfloat *)value);
}

void _glUniform4iv(GLint location, GLsizei count, intptr_t value)
{
	glUniform4iv(location, count, (const GLint *)value);
}

void _glUniform3iv(GLint location, GLsizei count, intptr_t value)
{
	glUniform3iv(location, count, (const GLint *)value);
}

void _glUniform2iv(GLint location, GLsizei count, intptr_t value)
{
	glUniform2iv(location, count, (const GLint *)value);
}

void _glUniform1iv(GLint location, GLsizei count, intptr_t value)
{
	glUniform1iv(location, count, (const GLint *)value);
}

void _glUniform4fv(GLint location, GLsizei count, intptr_t value)
{
	glUniform4fv(location, count, (const GLfloat *)value);
}

void _glUniform3fv(GLint location, GLsizei count, intptr_t value)
{
	glUniform3fv(location, count, (const GLfloat *)value);
}

void _glUniform2fv(GLint location, GLsizei count, intptr_t value)
{
	glUniform2fv(location, count, (const GLfloat *)value);
}

void _glUniform1fv(GLint location, GLsizei count, intptr_t value)
{
	glUniform1fv(location, count, (const GLfloat *)value);
}

void _glDrawElements(GLenum mode, GLsizei count, GLenum type, intptr_t indices)
{
	glDrawElements(mode, count, type, (const void*)indices);
}

void _glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, intptr_t pixels) {
    glTexImage2D(target, level, internalformat, width, height, border, format, type, (void*)pixels);
}

void _glShaderSource(GLuint shader, GLsizei count, const std::string& string, intptr_t length) {
    glShaderSource(shader, count, (const GLchar * const *)string.c_str(), (GLint *)length);
}

v8::Local<v8::Value> glGetParameter(GLenum pname)
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
            return v8::Int32::New(v8::Isolate::GetCurrent(), value);
		}
	//MAX_VERTEX_UNIFORM_VECTORS
    case 0x8DFB:
	//MAX_VARYING_VECTORS
    case 0x8DFC:
	//MAX_FRAGMENT_UNIFORM_VECTORS
    case 0x8DFD:
    //IMPLEMENTATION_COLOR_READ_FORMAT
    case 0x8B9B:
	//IMPLEMENTATION_COLOR_READ_TYPE
    case 0x8B9A:
	//UNPACK_COLORSPACE_CONVERSION_WEBGL
    case 0x9243:
		//TODO
        return v8::Int32::New(v8::Isolate::GetCurrent(), 0);

	//return uint
    case GL_STENCIL_BACK_VALUE_MASK:
    case GL_STENCIL_BACK_WRITEMASK:
    case GL_STENCIL_VALUE_MASK:
    case GL_STENCIL_WRITEMASK:
		{
			GLuint value;
			glGetIntegerv(pname, (GLint *)&value);
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
			return v8::Boolean::New(v8::Isolate::GetCurrent(), value);
		}
    //UNPACK_FLIP_Y_WEBGL:
    case 0x9240:
    //UNPACK_PREMULTIPLY_ALPHA_WEBGL:
    case 0x9241:
		{
			//TODO
			return v8::Boolean::New(v8::Isolate::GetCurrent(), false);
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
			return v8::Number::New(v8::Isolate::GetCurrent(), value);
		}

	}
	
	_glSetError(GL_INVALID_ENUM);
	return v8::Null(v8::Isolate::GetCurrent());
}

#define BIND_CLASS(x) v8pp::class_<x> class_##x(iso);class_##x.inherit<WebGLObject>();global->Set(v8pp::to_v8(iso, #x), class_##x.js_function_template()->GetFunction());

#define BIND_CLASS_WITH_MEMBER(x, member) v8pp::class_<x> class_##x(iso);\
	class_##x.inherit<WebGLObject>();\
	class_##x##member;\
global->Set(v8pp::to_v8(iso, #x), class_##x.js_function_template()->GetFunction());


//directly map function to native openGL function
void Bind_GL(v8::Isolate * iso)
{
    auto global = iso->GetCurrentContext()->Global();

	v8pp::class_<WebGLObject> class_WebGLObject(iso);
	global->Set(v8pp::to_v8(iso, "WebGLObject"), class_WebGLObject.create_object(iso));

	//WebGLActiveInfo
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

	v8pp::module module_GL(iso);
	module_GL//.set("test", &gl::test)
		.set("viewport", glViewport)
		.set("vertexAttribPointer", _glVertexAttribPointer)
		.set("vertexAttrib2fv", _glVertexAttrib2fv)
		.set("useProgram", glUseProgram)
		.set("uniformMatrix4fv", _glUniformMatrix4fv)
		.set("uniformMatrix3fv", _glUniformMatrix3fv)
		.set("uniformMatrix2fv", _glUniformMatrix2fv)
		.set("uniform4iv", _glUniform4iv)
		.set("uniform4i", glUniform4i)
		.set("uniform4fv", _glUniform4fv)
		.set("uniform4f", glUniform4f)
		.set("uniform3iv", _glUniform3iv)
		.set("uniform3i", glUniform3i)
		.set("uniform3fv", _glUniform3fv)
		.set("uniform3f", glUniform3f)
		.set("uniform2iv", _glUniform2iv)
		.set("uniform2i", glUniform2i)
		.set("uniform2fv", _glUniform2fv)
		.set("uniform2f", glUniform2f)
		.set("uniform1iv", _glUniform1iv)
		.set("uniform1i", glUniform1i)
		.set("uniform1fv", _glUniform1fv)
		.set("uniform1f", glUniform1f)
		.set("texParameteri", glTexParameteri)
		.set("texImage2D", _glTexImage2D)
		.set("shaderSource", _glShaderSource)
		.set("scissor", glScissor)
		.set("renderbufferStorage", glRenderbufferStorage)
		.set("pixelStorei", glPixelStorei)
		.set("linkProgram", glLinkProgram)
		.set("lineWidth", glLineWidth)
		.set("getUniformLocation", glGetUniformLocation)
		//.set("getShaderParameter", glGetShaderParameter)
		.set("getAttribLocation", glGetAttribLocation)
		.set("generateMipmap", glGenerateMipmap)
		.set("frontFace", glFrontFace)
		.set("framebufferTexture2D", glFramebufferTexture2D)
		.set("flush", glFlush)
		.set("enableVertexAttribArray", glEnableVertexAttribArray)
		.set("enable", glEnable)
		.set("drawElements", _glDrawElements)
		.set("disableVertexAttribArray", glDisableVertexAttribArray)
		.set("disable", glDisable)
		.set("depthMask", glDepthMask)
		.set("depthFunc", glDepthFunc)
		//.set("deleteTexture", glDeleteTexture)
		.set("deleteShader", glDeleteShader)
		//.set("deleteRenderbuffer", glDeleteRenderbuffer)
		.set("deleteProgram", glDeleteProgram)
		//.set("deleteFramebuffer", glDeleteFramebuffer)
		//.set("deleteBuffer", glDeleteBuffer)
		.set("cullFace", glCullFace)
		//.set("createTexture", glCreateTexture)
		.set("createShader", glCreateShader)
		//.set("createRenderbuffer", glCreateRenderbuffer)
		.set("createProgram", glCreateProgram)
		//.set("createFramebuffer", glCreateFramebuffer)
		//.set("createBuffer", glCreateBuffer)
		.set("compileShader", glCompileShader)
		.set("colorMask", glColorMask)
		.set("clearStencil", glClearStencil)
		.set("clearDepth", glClearDepth)
		.set("clearColor", glClearColor)
		.set("clear", glClear)
		//.set("bufferData", glBufferData)
		.set("blendFuncSeparate", glBlendFuncSeparate)
		.set("blendFunc", glBlendFunc)
		.set("blendEquationSeparate", glBlendEquationSeparate)
		.set("blendEquation", glBlendEquation)
		.set("bindRenderbuffer", glBindRenderbuffer)
		.set("bindFramebuffer", glBindFramebuffer)
		.set("bindBuffer", glBindBuffer)
		.set("bindAttribLocation", glBindAttribLocation)
		.set("attachShader", glAttachShader)
		.set("activeTexture", glActiveTexture)
		.set("validateProgram", glValidateProgram)
		//.set("texSubImage2D", glTexSubImage2D)
		.set("texParameterf", glTexParameterf)
		.set("stencilOpSeparate", glStencilOpSeparate)
		.set("stencilOp", glStencilOp)
		.set("stencilMaskSeparate", glStencilMaskSeparate)
		.set("stencilMask", glStencilMask)
		.set("stencilFuncSeparate", glStencilFuncSeparate)
		.set("stencilFunc", glStencilFunc)
		.set("sampleCoverage", glSampleCoverage)
		//.set("readPixels", glReadPixels)
		.set("polygonOffset", glPolygonOffset)
		.set("isTexture", glIsTexture)
		.set("isShader", glIsShader)
		.set("isRenderbuffer", glIsRenderbuffer)
		.set("isProgram", glIsProgram)
		.set("isFramebuffer", glIsFramebuffer)
		.set("isEnabled", glIsEnabled)
		//.set("isContextLost", glIsContextLost)
		.set("isBuffer", glIsBuffer)
		//.set("getVertexAttribOffset", glGetVertexAttribOffset)
		//.set("getVertexAttrib", glGetVertexAttrib)
		//.set("getUniform", glGetUniform)
		//.set("getTexParameter", glGetTexParameter)
		//.set("getSupportedExtensions", glGetSupportedExtensions)
		//.set("getShaderSource", glGetShaderSource)
		//.set("getShaderPrecisionFormat", glGetShaderPrecisionFormat)
		//.set("getShaderInfoLog", glGetShaderInfoLog)
		//.set("getRenderbufferParameter", glGetRenderbufferParameter)
		//.set("getProgramParameter", glGetProgramParameter)
		//.set("getProgramInfoLog", glGetProgramInfoLog)
		//.set("getParameter", glGetParameter)
		//.set("getFramebufferAttachmentParameter", glGetFramebufferAttachmentParameter)
		//.set("getExtension", glGetExtension)
		.set("getError", _glGetError)
		//.set("getContextAttributes", glGetContextAttributes)
		//.set("getBufferParameter", glGetBufferParameter)
		//.set("getAttachedShaders", glGetAttachedShaders)
		//.set("getActiveUniform", glGetActiveUniform)
		//.set("getActiveAttrib", glGetActiveAttrib)
		.set("drawArrays", glDrawArrays)
		.set("detachShader", glDetachShader)
		.set("depthRange", glDepthRange)
		.set("copyTexSubImage2D", glCopyTexSubImage2D)
		.set("copyTexImage2D", glCopyTexImage2D)
		//.set("compressedTexSubImage2D", glCompressedTexSubImage2D)
		//.set("compressedTexImage2D", glCompressedTexImage2D)
		.set("checkFramebufferStatus", glCheckFramebufferStatus)
		//.set("bufferSubData", glBufferSubData)
		.set("blendColor", glBlendColor)
		.set("bindTexture", glBindTexture)
		//.set("commit", glCommit)
		.set("finish", glFinish)
		.set("framebufferRenderbuffer", glFramebufferRenderbuffer)
		.set("hint", glHint)
		.set_const("GL_COLOR_BUFFER_BIT", GL_COLOR_BUFFER_BIT)
		;

	global->Set(v8pp::to_v8(iso, "gl"), module_GL.new_instance());
}