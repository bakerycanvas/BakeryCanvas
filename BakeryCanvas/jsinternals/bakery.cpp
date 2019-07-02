#include "bakery.h"

namespace BKJSInternals {
    v8pp::class_<Canvas>* CanvasPrototype = nullptr;
    v8pp::class_<Image>* ImagePrototype = nullptr;

    void initBakery() {
        auto isolate = v8::Isolate::GetCurrent();
        CanvasPrototype = new v8pp::class_<Canvas>(isolate);
        CanvasPrototype->set("getContext", &Canvas::getContext);
        CanvasPrototype->set("width", v8pp::property(&Canvas::width_getter, &Canvas::width_setter));
        CanvasPrototype->set("height", v8pp::property(&Canvas::height_getter, &Canvas::height_setter));
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
