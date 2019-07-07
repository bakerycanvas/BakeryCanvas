#include "bakery.h"

#include "2d/CanvasRenderingContext2d.h"
#include "canvas.h"
#include "image.h"
#include "log.h"
#include "settimeout.h"
#include "requestanimationframe.h"

namespace BKJSInternals {

    void initBakery() {
        auto isolate = v8::Isolate::GetCurrent();
        auto global = isolate->GetCurrentContext()->Global();

        // set global to `window`
        global->Set(v8pp::to_v8(isolate, "window"), global);

        // initialize Nan based class
        Context2d::Initialize(global);

        // create `bakery`
        v8pp::module bakery(isolate);

        // add class prototype
        Image::init(bakery);
        Canvas::init(bakery);

        // other functions
        bakery.set("log", log);
        bakery.set("setTimeout", setTimeout);
        bakery.set("setInterval", setInterval);
        bakery.set("clearTimeout", clearTimeout);
        bakery.set("clearInterval", clearInterval);
        bakery.set("requestAnimationFrame", requestAnimationFrame);
        bakery.set("cancelAnimationFrame", cancelAnimationFrame);

        // export `bakery`
        global->Set(v8pp::to_v8(isolate, "bakery"), bakery.new_instance());

        // init GL module and cache
        getGLmodule();
    }

}  // namespace BKJSInternals
