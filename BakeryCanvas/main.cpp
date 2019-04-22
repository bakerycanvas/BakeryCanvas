// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <libplatform/libplatform.h>

#include "Bind_GL.h"
#include "internals/bind.h"
#include "queue/queue.h"


#ifdef WIN32
#pragma comment(lib, "v8_monolith")
#pragma comment(lib, "glfw3dll")
#pragma comment(lib, "dbghelp")
#pragma comment(lib, "winmm")
#pragma comment(lib, "shlwapi")
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* InitWindow(int width = 800, int height = 600, const char* title = "Bakery")
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef WIN32
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#endif
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	char *errorbuf;
	auto error = glfwGetError((const char**)&errorbuf);
	if (window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return NULL;
	}
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}

void mainLoop(GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		BKQueue::tick();
	}
}

void deInitGLFW()
{
	glfwTerminate();
}

v8::Isolate *isolate;

void V8RunScript(v8::Local<v8::Context> v8_main_context, std::string scriptsrc, std::string &resultstr, std::string &exceptionstr)
{
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handle_scope(isolate);
	//v8::Local<v8::Context> context = v8::Context::New(isolate, NULL, v8_global);
	//v8::Context::Scope context_scope(context);
	v8::TryCatch tryHandler(isolate);
	v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, scriptsrc.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
	v8::MaybeLocal<v8::Script> maybescript = v8::Script::Compile(v8_main_context, source);
	if (maybescript.IsEmpty())
	{
		v8::Local<v8::Value> exception = tryHandler.Exception();
		v8::String::Utf8Value exception_utf8(isolate, exception);
		exceptionstr = *exception_utf8;
		resultstr.clear();
		return;
	}
	v8::Local<v8::Script> script = maybescript.ToLocalChecked();
	v8::MaybeLocal<v8::Value> result = script->Run(v8_main_context);
	if (result.IsEmpty())
	{
		v8::Local<v8::Value> exception = tryHandler.Exception();
		v8::String::Utf8Value exception_utf8(isolate, exception);
		exceptionstr = *exception_utf8;
		resultstr.clear();
	}
	else
	{
		v8::String::Utf8Value exception_utf8(isolate, result.ToLocalChecked());
		resultstr = *exception_utf8;
		exceptionstr.clear();
	}
}

int main(int argc, char* argv[])
{
	GLFWwindow *win = InitWindow();
	//init V8, must be in main, or caused 9999+ kinds of crashes

	std::string filename;
	printf("\n");
	for (int i = 1; i < argc; i++) {
		printf("arg[%d]:%s\n", i, argv[i]);
		if (i == 1) {
			filename = argv[1];
		}
	}

	printf("\n\n");

	std::string scriptText;
	if (!filename.empty()) {
		std::ifstream file;
		file.open(filename);

		if (!file) {
			printf("file read error\n");
			return 0;
		}

		std::ostringstream tmp;
		tmp << file.rdbuf();
		scriptText = tmp.str();
	}

	v8::V8::InitializeICUDefaultLocation(argv[0]);
	v8::V8::InitializeExternalStartupData(argv[0]);
	std::unique_ptr<v8::TracingController> tracing_controller = {};
	std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
	v8::V8::InitializePlatform(platform.get());
	v8::V8::Initialize();
	v8::Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
	isolate = v8::Isolate::New(create_params);
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handle_scope(isolate);
	v8::Local<v8::Context> v8_main_context = v8::Context::New(isolate);
	v8::Context::Scope context_scope(v8_main_context);
	Bind_GL(isolate);
	Bind_Internals(isolate);

	BKQueue::start();

	std::string result, exception;
	if (scriptText.empty()) {
		V8RunScript(v8_main_context, "gl.clearColor(0.2, 0.3, 0.3, 1.0);\n gl.clear(gl.GL_COLOR_BUFFER_BIT);", result, exception);
	} else {
		V8RunScript(v8_main_context, scriptText, result, exception);
	}
	printf("\n\n\nBakery Canvas running...\n\nresult:%s\nexceptions:%s\n", result.c_str(), exception.c_str());

	mainLoop(win);
	BKQueue::close();
	deInitGLFW();
	return 0;
}
