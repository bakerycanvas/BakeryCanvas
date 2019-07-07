#ifndef CANVAS_H
#define CANVAS_H

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

#include "2d/CanvasRenderingContext2d.h"
#include "Bind_GL.h"

namespace BKJSInternals {

    class Canvas {
        public:
        cairo_format_t format = CAIRO_FORMAT_ARGB32;
        int32_t width = 800;
        int32_t height = 600;

        Context2d* context2d = nullptr;

        cairo_surface_t* surface = nullptr;
        cairo_t* cairoContext = nullptr;

        Canvas();

        static v8pp::class_<Canvas>* CanvasPrototype;
        static void init(v8pp::module target);
        static v8::Local<v8::Object> createCanvas();

        void getContext(const v8::FunctionCallbackInfo<v8::Value>& args);
        void saveToPNG(const std::string& filename);

        int32_t getWidth();
        void setWidth(int32_t value);

        int32_t getHeight();
        void setHeight(int32_t value);

        unsigned char* getData();

        private:
    };

}  // namespace BKJSInternals

#endif  // CANVAS_H
