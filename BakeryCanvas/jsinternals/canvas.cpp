#include "canvas.h"

namespace BKJSInternals {

    v8pp::class_<Canvas>* Canvas::CanvasPrototype = nullptr;

    v8::Local<v8::Object> Canvas::createCanvas() {
        auto isolate = v8::Isolate::GetCurrent();
        return CanvasPrototype->create_object(isolate);
    };

    Canvas::Canvas() {
        // TODO: create when `.getContext('2d')` is called?
        this->surface = cairo_image_surface_create(this->format, this->width, this->height);
        this->cairoContext = cairo_create(this->surface);
    }

    void Canvas::init(v8pp::module target) {
        auto isolate = v8::Isolate::GetCurrent();
        CanvasPrototype = new v8pp::class_<Canvas>(isolate);
        CanvasPrototype->class_function_template()->SetClassName(v8pp::to_v8(isolate, "Canvas"));
        CanvasPrototype->set("getContext", &Canvas::getContext);
        CanvasPrototype->set("saveToPNG", &Canvas::saveToPNG);
        CanvasPrototype->set("width", v8pp::property(&Canvas::getWidth, &Canvas::setWidth));
        CanvasPrototype->set("height", v8pp::property(&Canvas::getHeight, &Canvas::setHeight));
        target.set("Canvas", *CanvasPrototype);
        target.set("createCanvas", Canvas::createCanvas);
    }

    void Canvas::getContext(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto isolate = args.GetIsolate();
        std::string type = v8pp::from_v8<std::string>(isolate, args[0]);
        std::transform(type.begin(), type.end(), type.begin(), ::tolower);
        if (type == "webgl") {
            //            auto global = isolate->GetCurrentContext()->Global();
            args.GetReturnValue().Set(getGLmodule());
            return;
        } else if (type == "2d") {
            if (this->context2d == nullptr) {
                //                auto self = args.This();
                //                v8::Local<v8::Value> args[1] = {
                //                    self
                //                };
                //                this->context = Context2d::constructor.Get(isolate)->GetFunction()->CallAsConstructor(isolate->GetCurrentContext(), 1, args).ToLocalChecked();
                this->context2d = new Context2d(this);
            }
            auto ret = v8pp::to_v8<Context2d*>(isolate, this->context2d);
            return args.GetReturnValue().Set(ret);
            //                return BKCanvas2D::getNewInstance(this->width, this->height);
        }
        isolate->ThrowException(v8::Exception::Error(v8pp::to_v8(isolate, "Unsupported context type.")));
    }

    void Canvas::saveToPNG(const std::string& filename) {
        cairo_surface_write_to_png(this->surface, filename.c_str());
    }

    int32_t Canvas::getWidth() {
        return this->width;
    }

    void Canvas::setWidth(int32_t value) {
        this->width = value;
    }

    int32_t Canvas::getHeight() {
        return this->height;
    }

    void Canvas::setHeight(int32_t value) {
        this->height = value;
    }

}  // namespace BKJSInternals
