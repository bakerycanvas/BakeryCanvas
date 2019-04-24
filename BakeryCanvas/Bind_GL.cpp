#include "Bind_GL.h"

static GLenum inner_GLError = GL_NO_ERROR;
static GLboolean use_InnerError = false;
static GLboolean glContextLost = false;

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

void _glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, intptr_t pixels)
{
    glTexImage2D(target, level, internalformat, width, height, border, format, type, (void*)pixels);
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
	case GL_MAX_VERTEX_UNIFORM_VECTORS:
	case GL_MAX_VARYING_VECTORS:
	case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
	case GL_IMPLEMENTATION_COLOR_READ_FORMAT:
	case GL_IMPLEMENTATION_COLOR_READ_TYPE:
	case GL_UNPACK_COLORSPACE_CONVERSION_WEBGL:
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
    case GL_UNPACK_FLIP_Y_WEBGL:
    case GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL:
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

#define CHECK_VALID(x) if(!x.isValid()){_glSetError(GL_INVALID_OPERATION);return;}
#define CHECK_VALID2(x,y) if(!x.isValid() || !y.isValid()){_glSetError(GL_INVALID_OPERATION);return;}
#define CHECK_VALID3(x,y,z) if(!x.isValid() || !y.isValid() || !z.isValid()){_glSetError(GL_INVALID_OPERATION);return;}


v8::Local<v8::Object> _glCreateTexture()
{
	auto texture = new WebGLTexture();
	glGenTextures(1, &texture->textureID);
	return v8pp::class_<WebGLTexture>::import_external(v8::Isolate::GetCurrent(), texture);
}

void _glDeleteTexture(WebGLTexture &texture)
{
	CHECK_VALID(texture);
	glDeleteTextures(1, &texture.textureID);
	texture.Invalidate();
}

v8::Local<v8::Object> _glCreateProgram()
{
	auto program = new WebGLProgram();
	program->program = glCreateProgram();
	return v8pp::class_<WebGLProgram>::import_external(v8::Isolate::GetCurrent(), program);
}

void _glDeleteProgram(WebGLProgram &program)
{
	CHECK_VALID(program);
	glDeleteProgram(program.program);
	program.Invalidate();
}

v8::Local<v8::Object> _glCreateShader(GLenum type)
{
	auto shader = new WebGLShader();
	shader->shader = glCreateShader(type);
	return v8pp::class_<WebGLShader>::import_external(v8::Isolate::GetCurrent(), shader);
}

void _glDeleteShader(WebGLShader &shader)
{
	CHECK_VALID(shader);
	glDeleteShader(shader.shader);
	shader.Invalidate();
}

v8::Local<v8::Object> _glCreateRenderbuffer()
{
	auto rbo = new WebGLRenderbuffer();
	glGenRenderbuffers(1, &rbo->rbo);
	return v8pp::class_<WebGLRenderbuffer>::import_external(v8::Isolate::GetCurrent(), rbo);
}

void _glDeleteRenderbuffer(WebGLRenderbuffer &rbo)
{
	CHECK_VALID(rbo);
	glDeleteRenderbuffers(1, &rbo.rbo);
	rbo.Invalidate();
}

v8::Local<v8::Object> _glCreateFramebuffer()
{
	auto fbo = new WebGLFramebuffer();
	glGenFramebuffers(1, &fbo->fbo);
	return v8pp::class_<WebGLFramebuffer>::import_external(v8::Isolate::GetCurrent(), fbo);
}

void _glDeleteFramebuffer(WebGLFramebuffer &fbo)
{
	CHECK_VALID(fbo);
	glDeleteFramebuffers(1, &fbo.fbo);
	fbo.Invalidate();
}

void _glShaderSource(WebGLShader &shader, const std::string &source)
{
	CHECK_VALID(shader);
	glShaderSource(shader.shader, 1, (const GLchar * const *)source.c_str(), NULL);
}

void _glCompileShader(WebGLShader &shader)
{
	CHECK_VALID(shader);
	glCompileShader(shader.shader);
}

void _glAttachShader(WebGLProgram &program, WebGLShader &shader)
{
	CHECK_VALID2(program, shader);
	glAttachShader(program.program, shader.shader);
}

void _glDetachShader(WebGLProgram &program, WebGLShader &shader)
{
	CHECK_VALID2(program, shader);
	glDetachShader(program.program, shader.shader);
}

void _glLinkProgram(WebGLProgram &program)
{
	CHECK_VALID(program);
	glLinkProgram(program.program);
}

void _glUseProgram(WebGLProgram &program)
{
	CHECK_VALID(program);
	glUseProgram(program.program);
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

	v8pp::module module_GL(iso);
	module_GL//.set("test", &gl::test)
		.set("viewport", glViewport)
		.set("vertexAttribPointer", _glVertexAttribPointer)
		.set("vertexAttrib2fv", _glVertexAttrib2fv)
		.set("useProgram", _glUseProgram)
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
		.set("linkProgram", _glLinkProgram)
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
		.set("deleteTexture", _glDeleteTexture)
		.set("deleteShader", _glDeleteShader)
		.set("deleteRenderbuffer", _glDeleteRenderbuffer)
		.set("deleteProgram", _glDeleteProgram)
		.set("deleteFramebuffer", _glDeleteFramebuffer)
		//.set("deleteBuffer", glDeleteBuffer)
		.set("cullFace", glCullFace)
		.set("createTexture", _glCreateTexture)
		.set("createShader", _glCreateShader)
		.set("createRenderbuffer", _glCreateRenderbuffer)
		.set("createProgram", _glCreateProgram)
		.set("createFramebuffer", _glCreateFramebuffer)
		//.set("createBuffer", glCreateBuffer)
		.set("compileShader", _glCompileShader)
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
		.set("attachShader", _glAttachShader)
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
		.set("isContextLost", _glIsContextLost)
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
		.set("detachShader", _glDetachShader)
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

	global->Set(v8pp::to_v8(iso, "gl"), module_GL.new_instance());
}