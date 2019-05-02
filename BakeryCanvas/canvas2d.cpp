#include "canvas2d.h"
#include "utils/color.h"
#include "utils/parse_font.h"

#define BK_PROPERTY_BIND(name) CanvasRenderingContext2DPrototype->set(#name, v8pp::property(&CanvasRenderingContext2D::name##_getter, &CanvasRenderingContext2D::name##_setter));

#define BK_METHOD_BIND(name) CanvasRenderingContext2DPrototype->set(#name, &CanvasRenderingContext2D::name);

#define BK_SET_WEAK(name)        \
    if (this->name != nullptr) { \
        this->name->SetWeak();   \
    }

namespace BKCanvas2D {

    CanvasRenderingContext2D::CanvasRenderingContext2D(int32_t width, int32_t height) {
        surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
        ctx = cairo_create(surface);

        this->canvas = nullptr;
        this->fillStyle = nullptr;
        this->strokeStyle = nullptr;
    }

    CanvasRenderingContext2D::~CanvasRenderingContext2D() {
        cairo_destroy(ctx);
        cairo_surface_destroy(surface);

        BK_SET_WEAK(canvas);
        BK_SET_WEAK(fillStyle);
        BK_SET_WEAK(strokeStyle);
    }

    auto CanvasRenderingContext2D::fillStyle_getter() {
        auto isolate = v8::Isolate::GetCurrent();
        return v8::Local<v8::Value>::New(isolate, *(this->fillStyle));
    }

    void CanvasRenderingContext2D::fillStyle_setter(v8::Local<v8::Value> value) {
        auto isolate = v8::Isolate::GetCurrent();

        if (value->IsString()) {
            short ok;
            auto rgba_str = v8pp::from_v8<const char*>(isolate, value->ToString(isolate));
            uint32_t rgba = rgba_from_string(rgba_str, &ok);
            if (!ok)
                return;

            auto rgba_struct = rgba_create(rgba);
            cairo_set_source_rgba(ctx, rgba_struct.r, rgba_struct.g, rgba_struct.b, rgba_struct.a);
        }

        // TODO: support gradient and pattern

        v8::Persistent<v8::Value>* persistentValue = new v8::Persistent<v8::Value>(isolate, value);
        this->fillStyle = persistentValue;
    }

    auto CanvasRenderingContext2D::font_getter() {
        auto isolate = v8::Isolate::GetCurrent();
        v8::EscapableHandleScope scope(isolate);
        return this->font;
    }

    void CanvasRenderingContext2D::font_setter(v8::Local<v8::Value> value) {
        auto isolate = v8::Isolate::GetCurrent();

        if (value->IsString()) {
            auto font_str = v8pp::from_v8<const char*>(isolate, value->ToString(isolate));
            auto font = parseCSSFont(font_str);

            // TODO: use local font file via freetype

            // use system font
            cairo_font_slant_t style = CAIRO_FONT_SLANT_NORMAL;
            cairo_font_weight_t weight = CAIRO_FONT_WEIGHT_NORMAL;
            if (font->style == "italic") {
                style = CAIRO_FONT_SLANT_ITALIC;
            } else if (font->style == "oblique") {
                style = CAIRO_FONT_SLANT_OBLIQUE;
            }
            // TODO: better weight support?
            if (font->weight == "bold" || font->weight == "bolder" || font->weight > "400") {
                weight = CAIRO_FONT_WEIGHT_BOLD;
            }
            cairo_select_font_face(ctx, font->family, style, weight);

            this->font = font_str;
        }
    }

    auto CanvasRenderingContext2D::lineWidth_getter() {
        return this->lineWidth;
    }

    void CanvasRenderingContext2D::lineWidth_setter(v8::Local<v8::Value> value) {
        auto isolate = v8::Isolate::GetCurrent();
        if (value->IsNumber()) {
            auto width = v8pp::from_v8<float>(isolate, value->ToNumber(isolate));
            cairo_set_line_width(ctx, width);
            this->lineWidth = width;
        }
    }

    // public methods
    void CanvasRenderingContext2D::beginPath() {
        cairo_new_path(ctx);
    }

    void CanvasRenderingContext2D::closePath() {
        cairo_close_path(ctx);
    }

    void CanvasRenderingContext2D::moveTo(double x, double y) {
        cairo_move_to(ctx, x, y);
    }

    void CanvasRenderingContext2D::lineTo(double x, double y) {
        cairo_line_to(ctx, x, y);
    }

    void CanvasRenderingContext2D::stroke() {
        cairo_stroke(ctx);
        cairo_surface_write_to_png(surface, "test.png");
    }

    void CanvasRenderingContext2D::fillRect(double x, double y, double w, double h) {
        cairo_rectangle(ctx, x, y, w, h);
        cairo_fill(ctx);
    }

    void CanvasRenderingContext2D::strokeRect(double x, double y, double w, double h) {
        cairo_rectangle(ctx, x, y, w, h);
        cairo_stroke(ctx);
    }

    void init(int width, int height) {
        cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 400, 400);
        cairo_t* cr = cairo_create(surface);

        // ctx.lineWidth = 10;

        // // Wall
        // ctx.strokeRect(75, 140, 150, 110);

        // // Door
        // ctx.fillRect(130, 190, 40, 60);

        // // Roof
        // ctx.moveTo(50, 140);
        // ctx.lineTo(150, 60);
        // ctx.lineTo(250, 140);
        // ctx.closePath();
        // ctx.stroke();

        // cairo_ft_font_face_create_for_ft_face()

        // cairo_set_source_rgb(cr, 1, 1, 1);
        // cairo_paint(cr);

        cairo_set_source_rgb(cr, 0, 0, 0);

        cairo_set_line_width(cr, 10);
        cairo_rectangle(cr, 75, 140, 150, 110);
        cairo_stroke(cr);
        // cairo_paint(cr);

        cairo_rectangle(cr, 130, 190, 40, 60);
        cairo_fill(cr);
        // cairo_paint(cr);

        cairo_move_to(cr, 50, 140);
        cairo_line_to(cr, 150, 60);
        cairo_line_to(cr, 250, 140);
        cairo_close_path(cr);
        cairo_stroke(cr);
        // cairo_paint(cr);

        FT_Library value;
        FT_Error status;
        FT_Face face;

        int size = 200;

        const char* filename = "/System/Library/Fonts/PingFang.ttc";

        status = FT_Init_FreeType(&value);
        if (status != 0) {
            fprintf(stderr, "Error %d opening library.\n", status);
            exit(EXIT_FAILURE);
        }
        status = FT_New_Face(value, filename, 0, &face);
        if (status != 0) {
            fprintf(stderr, "Error %d opening %s.\n", status, filename);
            exit(EXIT_FAILURE);
        }

        // auto x = cairo_ft_font_face_create_for_ft_face(face, 0);
        // cairo_set_font_face(cr, x);

        cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 24);
        cairo_move_to(cr, 100, 350);
        cairo_show_text(cr, "test测试");

        // printf("%s", cairo_toy_font_face_get_family())

        // cairo_set_source_rgb(cr, 1, 1, 1);
        // cairo_paint(cr);
        // cairo_set_source_rgb(cr, 0, 0, 0);
        // for (int i = 0; i <= 10000; i++) {
        //     double x = 200 + cos(2 * M_PI * i / 500) * 70 + cos(2 * M_PI * i / 10000) * 110;
        //     double y = 200 + sin(2 * M_PI * i / 500) * 70 + sin(2 * M_PI * i / 10000) * 110;
        //     if (i == 0)
        //         cairo_move_to(cr, x, y);
        //     else
        //         cairo_line_to(cr, x, y);
        // }
        // cairo_close_path(cr);
        // cairo_stroke(cr);
        cairo_surface_write_to_png(surface, "spiral.png");
        cairo_destroy(cr);
        cairo_surface_destroy(surface);
    }

    v8pp::class_<CanvasRenderingContext2D>* CanvasRenderingContext2DPrototype;
    void bind() {
        auto isolate = v8::Isolate::GetCurrent();
        CanvasRenderingContext2DPrototype = new v8pp::class_<CanvasRenderingContext2D>(isolate);
        CanvasRenderingContext2DPrototype->ctor<int32_t, int32_t>();
        BK_PROPERTY_BIND(fillStyle);
        BK_PROPERTY_BIND(lineWidth);
        BK_METHOD_BIND(beginPath);
        BK_METHOD_BIND(closePath);
        BK_METHOD_BIND(moveTo);
        BK_METHOD_BIND(lineTo);
        BK_METHOD_BIND(stroke);
        BK_METHOD_BIND(fillRect);
        BK_METHOD_BIND(strokeRect);
    }
    v8::Local<v8::Object> getNewInstance(int32_t width, int32_t height) {
        auto isolate = v8::Isolate::GetCurrent();
        v8::EscapableHandleScope scope(isolate);
        auto instance = CanvasRenderingContext2DPrototype->create_object(isolate, width, height);
        return scope.Escape(instance);
    }
}  // namespace BKCanvas2D
