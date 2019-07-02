#pragma once

#include <cairo-ft.h>
#include <cairo.h>
#include <freetype/config/ftheader.h>
#include <ft2build.h>
#define _USE_MATH_DEFINES
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
#include <math.h>
#include <string>
#include <v8.h>

#define BK_PROPERTY_DEF(type, name) \
    type name;                      \
    auto name##_getter();           \
    void name##_setter(v8::Local<v8::Value>);

namespace BKCanvas2D {
    void init(int width, int height);
    void bind();
    v8::Local<v8::Object> getNewInstance(int32_t width, int32_t height);
    class CanvasRenderingContext2D {
        public:
        CanvasRenderingContext2D(int32_t width, int32_t height);
        ~CanvasRenderingContext2D();

        BK_PROPERTY_DEF(v8::Persistent<v8::Object>*, canvas);
        BK_PROPERTY_DEF(v8::Persistent<v8::Value>*, fillStyle);

        BK_PROPERTY_DEF(std::string, font);
        BK_PROPERTY_DEF(float, globalAlpha);
        BK_PROPERTY_DEF(std::string, globalCompositeOperation);
        BK_PROPERTY_DEF(bool, imageSmoothingEnabled);
        BK_PROPERTY_DEF(std::string, imageSmoothingQuality);  // exp
        BK_PROPERTY_DEF(std::string, lineCap);                      // "butt" || "round" || square"
        BK_PROPERTY_DEF(float, lineDashOffset);
        BK_PROPERTY_DEF(std::string, lineJoin);
        BK_PROPERTY_DEF(float, lineWidth);
        BK_PROPERTY_DEF(float, miterLimit);
        BK_PROPERTY_DEF(float, shadowBlur);
        BK_PROPERTY_DEF(std::string, shadowColor);
        BK_PROPERTY_DEF(float, shadowOffsetX);
        BK_PROPERTY_DEF(float, shadowOffsetY);
        BK_PROPERTY_DEF(v8::Persistent<v8::Object>*, strokeStyle);
        BK_PROPERTY_DEF(std::string, textAlign);
        BK_PROPERTY_DEF(std::string, textBaseline);

        void arc();
        void arcTo();
        void beginPath();
        void bezierCurveTo();
        void clearRect();
        void clip();
        void closePath();
        void createImageData();
        void createLinearGradient();
        void createPattern();
        void createRadialGradient();
        // void drawFocusIfNeeded();
        void drawImage();
        void ellipse();
        void fill();
        void fillRect(double x, double y, double w, double h);
        void fillText();
        void getImageData();
        void getLineDash();
        void isPointInPath();
        void isPointInStroke();
        void lineTo(double x, double y);
        void measureText();
        void moveTo(double x, double y);
        void putImageData();
        void quadraticCurveTo();
        void rect();
        void restore();
        void rotate();
        void save();
        void scale();
        void setLineDash();
        void setTransform();
        void stroke();
        void strokeRect(double x, double y, double w, double h);
        void strokeText();
        void transform();
        void translate();

        private:
        cairo_surface_t* surface;
        cairo_t* ctx;
    };
}  // namespace BKCanvas2D
