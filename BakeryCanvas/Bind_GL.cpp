#include "Bind_GL.h"

#include "GL.h"

decltype(gl::_GL_COLOR_BUFFER_BIT) Get__GL_COLOR_BUFFER_BIT()
{
	return gl::_GL_COLOR_BUFFER_BIT;
}

void Bind_GL(v8::Isolate * iso)
{
	v8pp::module module_GL(iso);
	module_GL.set("test", &gl::test)
		.set("glViewport", &gl::glViewport)
		.set("glClearColor", &gl::glClearColor)
		.set("glClear", &gl::glClear)
		.set("GL_COLOR_BUFFER_BIT", v8pp::property(Get__GL_COLOR_BUFFER_BIT));

	iso->GetCurrentContext()->Global()->Set(v8pp::to_v8(iso, "gl"), module_GL.new_instance());
}