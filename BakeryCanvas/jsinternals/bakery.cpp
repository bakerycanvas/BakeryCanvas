#include "bakery.h"

namespace BKJSInternals {
    class Canvas {
        public:
        v8::Local<v8::Value> getContext(std::string type) {
            auto isolate = v8::Isolate::GetCurrent();
            std::transform(type.begin(), type.end(), type.begin(), ::tolower);
            if (type == "webgl") {
                auto global = isolate->GetCurrentContext()->Global();
                return global->Get(v8pp::to_v8(isolate, "gl"));
            }
            return v8pp::throw_ex(isolate, "Unsupported context type.");
        }
    };

    class Audio {};
    class Image {};

    v8pp::class_<Canvas>* CanvasPrototype;

    void initBakery() {
        auto isolate = v8::Isolate::GetCurrent();
        CanvasPrototype = new v8pp::class_<Canvas>(isolate);
        CanvasPrototype->set("getContext", &Canvas::getContext);

        v8pp::module bakery(isolate);
        bakery.set("createCanvas", createCanvas);

        auto global = isolate->GetCurrentContext()->Global();
        global->Set(v8pp::to_v8(isolate, "bakery"), bakery.new_instance());
    }

    v8::Local<v8::Object> createCanvas(){
        auto isolate = v8::Isolate::GetCurrent();
        return CanvasPrototype->create_object(isolate);
    };
}  // namespace BKJSInternals
