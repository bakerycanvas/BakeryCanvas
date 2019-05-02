#include "bakery.h"

namespace BKJSInternals {
    class Canvas {
        public:
        v8::Local<v8::Value> getContext(std::string type) {
            auto isolate = v8::Isolate::GetCurrent();
            std::transform(type.begin(), type.end(), type.begin(), ::tolower);
            if (type == "webgl") {
                auto global = isolate->GetCurrentContext()->Global();
                return getGLmodule();
            }
            return v8pp::throw_ex(isolate, "Unsupported context type.");
        }
    };

    class Audio {};
    class Image {
        public:
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

        private:
        const char* data;
        void loadImage() {
            if (!this->onload->IsEmpty()) {
                auto isolate = v8::Isolate::GetCurrent();
                v8::HandleScope scope(isolate);
                v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate, *(this->onload));
                v8pp::call_v8(isolate, callback, isolate->GetCurrentContext()->Global());
            }
        }
    };

    v8pp::class_<Canvas>* CanvasPrototype;
    v8pp::class_<Image>* ImagePrototype;

    void initBakery() {
        auto isolate = v8::Isolate::GetCurrent();
        CanvasPrototype = new v8pp::class_<Canvas>(isolate);
        CanvasPrototype->set("getContext", &Canvas::getContext);
        ImagePrototype = new v8pp::class_<Image>(isolate);
        ImagePrototype->set("src", v8pp::property(&Image::src_getter, &Image::src_setter));
        ImagePrototype->set("onload", v8pp::property(&Image::onload_getter, &Image::onload_setter));

        v8pp::module bakery(isolate);
        bakery.set("createCanvas", createCanvas);
        bakery.set("createImage", createImage);

        auto global = isolate->GetCurrentContext()->Global();
        global->Set(v8pp::to_v8(isolate, "bakery"), bakery.new_instance());

        // init GL module and cache
        getGLmodule();
    }

    v8::Local<v8::Object> createCanvas() {
        auto isolate = v8::Isolate::GetCurrent();
        return CanvasPrototype->create_object(isolate);
    };

    v8::Local<v8::Object> createImage() {
        auto isolate = v8::Isolate::GetCurrent();
        return ImagePrototype->create_object(isolate);
    };
}  // namespace BKJSInternals
