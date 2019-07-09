#include "image.h"
#include "imagedecoder/imagedecoder.h"

namespace BKJSInternals {

    v8pp::class_<Image>* Image::ImagePrototype = nullptr;

    v8::Local<v8::Object> Image::createImage(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto isolate = v8::Isolate::GetCurrent();
        return ImagePrototype->create_object<const v8::FunctionCallbackInfo<v8::Value>&>(isolate, args);
    };

    Image::Image(const v8::FunctionCallbackInfo<v8::Value>& args) {
        //        this->surface = cairo_image_surface_create(this->format, this->width, this->height);
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
        ImagePrototype->set("width", v8pp::property(&Image::getWidth, &Image::setWidth));
        ImagePrototype->set("height", v8pp::property(&Image::getHeight, &Image::setHeight));
        ImagePrototype->set("complete", v8pp::property(&Image::getComplete));
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

    bool Image::getComplete() {
        return this->complete;
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
            auto func = new v8::Persistent<v8::Function>(isolate, value);
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

    unsigned char* Image::getData() {
        return this->data;
    }

    void Image::loadImage() {
        auto img = ImageDecoder::create(this->src.c_str());
        this->surface = cairo_image_surface_create_for_data(img->getPixelData(), CAIRO_FORMAT_ARGB32, img->getWidth(), img->getHeight(), img->getPitch());
        cairo_surface_flush(this->surface);
        delete img;
        this->data = cairo_image_surface_get_data(this->surface);
        this->width = cairo_image_surface_get_width(this->surface);
        this->height = cairo_image_surface_get_height(this->surface);
        this->cairoContext = cairo_create(this->surface);
        this->complete = true;
        if (this->onload != nullptr) {
            auto isolate = v8::Isolate::GetCurrent();
            v8::HandleScope scope(isolate);
            v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate, *(this->onload));
            v8pp::call_v8(isolate, callback, isolate->GetCurrentContext()->Global());
        }
    }
}  // namespace BKJSInternals
