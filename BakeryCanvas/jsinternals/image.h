#ifndef IMAGE_H
#define IMAGE_H

#include "cairo.h"
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

namespace BKJSInternals {

    class Image {
        public:
        cairo_format_t format = CAIRO_FORMAT_ARGB32;
        bool complete = false;
        std::string src;
        int32_t width = 800;
        int32_t height = 600;
        v8::Persistent<v8::Function>* onload = nullptr;

        cairo_surface_t* surface = nullptr;
        cairo_t* cairoContext = nullptr;

        explicit Image(const v8::FunctionCallbackInfo<v8::Value>&);

        static v8pp::class_<Image>* ImagePrototype;
        static void init(v8pp::module target);
        static v8::Local<v8::Object> createImage(const v8::FunctionCallbackInfo<v8::Value>& args);

        void setSrc(std::string value);
        std::string getSrc();

        int32_t getWidth();
        void setWidth(int32_t value);

        int32_t getHeight();
        void setHeight(int32_t value);

        void setOnload(v8::Local<v8::Function> value);
        void getOnload(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);

        ~Image();

        private:
        void loadImage();
    };

}  // namespace BKJSInternals

#endif  // IMAGE_H
