#pragma once

#include "canvas2d.h"
// #include "context2d.h"
#include "v8pp/call_from_v8.hpp"
#include "v8pp/call_v8.hpp"
#include "v8pp/class.hpp"
#include "v8pp/config.hpp"
#include "v8pp/context.hpp"
#include "v8pp/json.hpp"
#include "v8pp/module.hpp"
#include "v8pp/object.hpp"
#include "v8pp/persistent.hpp"
#include "v8pp/throw_ex.hpp"
#include "v8pp/utility.hpp"

#include "../Bind_GL.h"

namespace BKJSInternals {
        class Canvas {
        public:
        v8::Local<v8::Value> getContext(std::string type) {
            auto isolate = v8::Isolate::GetCurrent();
            std::transform(type.begin(), type.end(), type.begin(), ::tolower);
            if (type == "webgl") {
                auto global = isolate->GetCurrentContext()->Global();
                return getGLmodule();
            } else if (type == "2d") {
                return BKCanvas2D::getNewInstance(this->width, this->height);
            }
            return isolate->ThrowException(v8::Exception::Error(v8pp::to_v8(isolate, "Unsupported context type.")));
        }
        int32_t width_getter() {
            return this->width;
        }
        void width_setter(int32_t value) {
            this->width = value;
        }
        int32_t height_getter() {
            return this->height;
        }
        void height_setter(int32_t value) {
            this->height = value;
        }

        private:
        int32_t width = 800;
        int32_t height = 600;
    };

    class Audio {};
    class Image {
        public:
        bool complete = false;
        std::string src;
        v8::Persistent<v8::Function>* onload;
        void src_setter(std::string value) {
            this->src = value;
            this->loadImage();
        }
        std::string src_getter() {
            return this->src;
        }
        void onload_setter(v8::Local<v8::Function> value) {
            auto isolate = v8::Isolate::GetCurrent();
            if (this->onload != nullptr) {
                this->onload->SetWeak();
            }

            v8::Persistent<v8::Function>* func = new v8::Persistent<v8::Function>(isolate, value);
            this->onload = func;
        }
        v8::Persistent<v8::Function>* onload_getter() {
            return this->onload;
        }

        ~Image(){};

        private:
        const char* data;
        void loadImage() {
            this->complete = true;
            if (!this->onload->IsEmpty()) {
                auto isolate = v8::Isolate::GetCurrent();
                v8::HandleScope scope(isolate);
                v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate, *(this->onload));
                v8pp::call_v8(isolate, callback, isolate->GetCurrentContext()->Global());
            }
        }
    };

    extern v8pp::class_<Canvas>* CanvasPrototype;
    extern v8pp::class_<Image>* ImagePrototype;

    void initBakery();
    void createAudio();
    v8::Local<v8::Object> createCanvas();
    v8::Local<v8::Object> createImage();
    void loadFileLocal();
    void loadFileRemote();
    void getSystemInfo();
    void writeStorageLocal();
    void request();
    void addEventListener();
}  // namespace BKJSInternals