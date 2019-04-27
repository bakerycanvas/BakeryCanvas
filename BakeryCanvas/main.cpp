// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <libplatform/libplatform.h>
#include <list>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#include "Bind_GL.h"
#include "jsinternals/bind.h"
#include "queue/queue.h"
#include "system.h"

#ifdef WIN32
#pragma comment(lib, "v8_monolith")
#pragma comment(lib, "glfw3dll")
// use shared lib
#pragma comment(lib, "libuv")
#pragma comment(lib, "dbghelp")
#pragma comment(lib, "winmm")
#pragma comment(lib, "shlwapi")
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* window;

GLFWwindow* InitWindow(int width = 800, int height = 600, const char* title = "Bakery") {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef WIN32
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    char* errorbuf;
    auto error = glfwGetError((const char**)&errorbuf);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return NULL;
    }
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

extern GLenum _glGetError();

void mainLoop(uv_idle_t* handle) {
    if (!glfwWindowShouldClose(window)) {
        if (shouldSwapBuffer())
        {
            if (getCurrentContextAttributes()->preserveDrawingBuffer)
            {
                //TODO:shit
            }
            glfwSwapBuffers(window);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            clearSwapBufferTag();
        }
        glfwPollEvents();
    } else {
        BKQueue::stop();
    }
}

void deInitGLFW() {
    glfwTerminate();
}

v8::Isolate* isolate;

void V8RunScript(v8::Local<v8::Context> v8_main_context, const std::string& scriptsrc, const std::string& filename, std::string& resultstr, std::string& exceptionstr) {
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    // v8::Local<v8::Context> context = v8::Context::New(isolate, NULL, v8_global);
    // v8::Context::Scope context_scope(context);
    v8::TryCatch tryHandler(isolate);
    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, scriptsrc.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
    v8::ScriptOrigin origin(v8::String::NewFromUtf8(isolate, filename.c_str(), v8::NewStringType::kNormal).ToLocalChecked());
    v8::MaybeLocal<v8::Script> maybescript = v8::Script::Compile(v8_main_context, source, !filename.empty() ? &origin : nullptr);
    if (maybescript.IsEmpty()) {
        v8::Local<v8::Value> exception = tryHandler.StackTrace(v8_main_context).ToLocalChecked();
        v8::String::Utf8Value exception_utf8(isolate, exception);
        exceptionstr = *exception_utf8;
        resultstr.clear();
        return;
    }
    v8::Local<v8::Script> script = maybescript.ToLocalChecked();
    v8::MaybeLocal<v8::Value> result = script->Run(v8_main_context);
    if (result.IsEmpty()) {
        v8::MaybeLocal<v8::Value> maybe_exception = tryHandler.StackTrace(v8_main_context);
        v8::Local<v8::Value> exception;
        if (maybe_exception.IsEmpty()) {
            exception = tryHandler.Exception();
        } else {
            exception = maybe_exception.ToLocalChecked();
        }
        v8::String::Utf8Value exception_utf8(isolate, exception);
        exceptionstr = *exception_utf8;
        resultstr.clear();
    } else {
        v8::String::Utf8Value exception_utf8(isolate, result.ToLocalChecked());
        resultstr = *exception_utf8;
        exceptionstr.clear();
    }
}

int main(int argc, char* argv[]) {
    GLFWwindow* win = InitWindow();
    // init V8, must be in main, or caused 9999+ kinds of crashes

    std::string filename;
    printf("\n");
    for (int i = 1; i < argc; i++) {
        printf("arg[%d]:%s\n", i, argv[i]);
        if (i == 1) {
            filename = argv[1];
        }
    }

    printf("Bakery Canvas running...\n\n");

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
    std::string exceptionFilename;

    if (filename.empty()) {
        V8RunScript(v8_main_context, "var s=gl.createShader(gl.VERTEX_SHADER);gl.getShaderParameter(s,0)", "", result, exception);
        if (result.length() > 0) {
            printf("result:%s\n", result.c_str());
        }
    } else {
        // get current working directory
        std::string cwd = filename.substr(0, filename.find_last_of('/'));
        std::ifstream entryFile;
        std::string entryFileName = cwd + "/entry.json";
        entryFile.open(entryFileName);

        std::list<std::string> fileList;

        if (!entryFile) {
            printf("No entry.json find at %s, try to load single file %s\n", entryFileName.c_str(), filename.c_str());
            fileList.push_back(filename);
        } else {
            v8::Local<v8::Context> tempContext;
            std::ostringstream tmp;
            tmp << entryFile.rdbuf();
            v8::Local<v8::String> code = v8::String::NewFromUtf8(isolate, tmp.str().c_str(), v8::NewStringType::kNormal).ToLocalChecked();
            v8::Local<v8::Value> entryValue = v8::JSON::Parse(tempContext, code).ToLocalChecked();
            v8::Local<v8::Object> entryObject = v8::Local<v8::Object>::Cast(entryValue);
            if (!entryObject.IsEmpty()) {
                v8::Local<v8::String> keyName = v8::String::NewFromUtf8(isolate, "files", v8::NewStringType::kNormal).ToLocalChecked();
                v8::Local<v8::Array> fileArray = v8::Local<v8::Array>::Cast(entryObject->Get(keyName));
                for (int i = 0; i < fileArray->Length(); i++) {
                    v8::Local<v8::String> fileValue = v8::Local<v8::String>::Cast(fileArray->Get(i));
                    std::string fileName = *v8::String::Utf8Value(isolate, fileValue);
                    printf("%s\n", (cwd + "/" + fileName).c_str());
                    fileList.push_back(cwd + "/" + fileName);
                }
            }
        }

        for (auto it = fileList.begin(); it != fileList.end(); it++) {
            std::string filename = *it;
            std::string scriptText;
            std::ifstream file;
            file.open(filename);
            exceptionFilename = filename;

            if (!file) {
                std::string head = "读取文件失败：";
                BKSystem::showMessage("Bakery Canvas Exception", (head + filename).c_str(), BKSystem::MessageLevel::ERROR);
                printf("file read error\n");
                return 0;
            }

            std::ostringstream tmp;
            tmp << file.rdbuf();
            scriptText = tmp.str();
            V8RunScript(v8_main_context, scriptText, filename, result, exception);
            if (!exception.empty()) {
                break;
            }
        }
    }

    if (exception.length() > 0) {
        // avoid stucked by large stack trace
        auto safeExceptionString = exception.substr(0, 2048);
        BKSystem::showMessage("Bakery Canvas Exception", (exceptionFilename + "\n" + safeExceptionString).c_str(), BKSystem::MessageLevel::ERROR);
        printf("Uncaught exception:\n%s\n\n", safeExceptionString.c_str());
    } else {
        uv_idle_t mainloop_handle;
        uv_idle_init(uv_default_loop(), &mainloop_handle);
        uv_idle_start(&mainloop_handle, mainLoop);
        BKQueue::tick();
    }

    BKQueue::close();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    deInitGLFW();
    printf("Bakery Canvas terminated.\n");
    return 0;
}
