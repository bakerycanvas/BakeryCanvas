#include "Bind_GL.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>



decltype(GL_COLOR_BUFFER_BIT) Get_GL_COLOR_BUFFER_BIT()
{
	return GL_COLOR_BUFFER_BIT;
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

//directly map function to native openGL function
void Bind_GL(v8::Isolate * iso)
{
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
		//.set("texImage2D", glTexImage2D)
		//.set("shaderSource", glShaderSource)
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
		.set("getError", glGetError)
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
		.set("GL_COLOR_BUFFER_BIT", v8pp::property(Get_GL_COLOR_BUFFER_BIT));

	iso->GetCurrentContext()->Global()->Set(v8pp::to_v8(iso, "gl"), module_GL.new_instance());
}