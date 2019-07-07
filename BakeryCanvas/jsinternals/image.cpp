#include "image.h"

namespace BKJSInternals {

    v8pp::class_<Image>* Image::ImagePrototype = nullptr;

    v8::Local<v8::Object> Image::createImage(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto isolate = v8::Isolate::GetCurrent();
        return ImagePrototype->create_object<const v8::FunctionCallbackInfo<v8::Value>&>(isolate, args);
    };

    Image::Image(const v8::FunctionCallbackInfo<v8::Value>& args) {
        //        this->surface = cairo_image_surface_create(this->format, this->width, this->height);
        this->surface = cairo_image_surface_create_from_png("/Users/Icemic/Workspace/seri_icon.png");
        this->cairoContext = cairo_create(this->surface);
    }

    Image::~Image() {
        if (this->onload != nullptr) {
            this->onload->SetWeak();
        }
    }

    void Image::init(v8pp::module target) {
        auto isolate = v8::Isolate::GetCurrent();
        ImagePrototype = new v8pp::class_<Image>(isolate);
        ImagePrototype->ctor<const v8::FunctionCallbackInfo<v8::Value>&>();
        ImagePrototype->class_function_template()->SetClassName(v8pp::to_v8(isolate, "Image"));
        ImagePrototype->set("src", v8pp::property(&Image::getSrc, &Image::setSrc));
        ImagePrototype->set("onload", v8pp::property(&Image::getOnload, &Image::setOnload));
        target.set("Image", *ImagePrototype);
        target.set("createImage", Image::createImage);
    }

    void Image::setSrc(std::string value) {
        this->src = value;
        this->loadImage();
    }

    std::string Image::getSrc() {
        return this->src;
    }

    void Image::setOnload(v8::Local<v8::Function> value) {
        auto isolate = v8::Isolate::GetCurrent();
        if (this->onload != nullptr) {
            this->onload->SetWeak();
        }

        if (value->IsNullOrUndefined()) {
            // TODO: should use v8::Null(isolate)
            this->onload = nullptr;
        } else {
            v8::Persistent<v8::Function>* func = new v8::Persistent<v8::Function>(isolate, value);
            this->onload = func;
        }
    }

    void Image::getOnload(v8::Local<v8::String> name, v8::PropertyCallbackInfo<v8::Value> const& info) {
        if (this->onload == nullptr) {
            // TODO: null or undefined?
            return info.GetReturnValue().SetNull();
        }
        auto isolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(isolate);
        v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate, *(this->onload));
        info.GetReturnValue().Set(callback);
    }

    int32_t Image::getWidth() {
        return this->width;
    }

    void Image::setWidth(int32_t value) {
        this->width = value;
    }

    int32_t Image::getHeight() {
        return this->height;
    }

    void Image::setHeight(int32_t value) {
        this->height = value;
    }

    void Image::loadImage() {
        this->complete = true;
        if (this->onload != nullptr) {
            auto isolate = v8::Isolate::GetCurrent();
            v8::HandleScope scope(isolate);
            v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate, *(this->onload));
            v8pp::call_v8(isolate, callback, isolate->GetCurrentContext()->Global());
        }
    }
}  // namespace BKJSInternals
