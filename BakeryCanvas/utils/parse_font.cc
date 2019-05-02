/**
 * From https://github.com/Automattic/node-canvas/blob/master/lib/parse-font.js
 */

#include "parse_font.h"

std::string WEIGHTS = "bold|bolder|lighter|[1-9]00";
std::string STYLES = "italic|oblique";
std::string VARIANTS = "small-caps";
std::string STRETCHES = "ultra-condensed|extra-condensed|condensed|semi-condensed|semi-expanded|expanded|extra-expanded|ultra-expanded";
std::string UNITS = "px|pt|pc|in|cm|mm|%|em|ex|ch|rem|q";
std::string STRING = "'([^']+)'|\"([^\"]+)\"|[\\\\w\\\\s-]+";

/**
 * Cache font parsing.
 * No remove, so won't release memory.
 */
std::unordered_map<const char*, fontinfo*> cache;

int defaultHeight = 16;  // pt, common browser default

auto WEIGHT_REG = std::regex("(" + WEIGHTS + ") +", std::regex::icase);
auto STYLE_REG = std::regex("(" + STYLES + ") +", std::regex::icase);
auto VARIANT_REG = std::regex("(" + VARIANTS + ") +", std::regex::icase);
auto STRETCH_REG = std::regex("(" + STRETCHES + ") +", std::regex::icase);
auto SIZE_FAMILY_REG = std::regex("([\\d\\.]+)(" + UNITS + ") *" + "((?:" + STRING + ")( *, *(?:" + STRING + "))*)");

fontinfo* parseCSSFont(const char* str) {
    auto font = cache[str];
    if (reinterpret_cast<size_t>(font) != 0) {
        return font;
    }

    std::cmatch sizeFamily;
    auto sizeFamilySuccess = std::regex_match(str, sizeFamily, SIZE_FAMILY_REG);
    if (!sizeFamilySuccess) {
        return nullptr;  // invalid
    }

    font = (fontinfo*)malloc(sizeof(fontinfo));

    font->weight = "normal";
    font->style = "normal";
    font->stretch = "normal";
    font->variant = "normal";
    font->size = atof(sizeFamily[1].str().c_str());
    font->unit = sizeFamily[2].str().c_str();
    font->family = std::regex_replace(std::regex_replace(sizeFamily[3].str().c_str(), std::regex("[\"']"), ""), std::regex(" *, * "), "").c_str();

    // Optional, unordered properties.
    std::cmatch weight, style, variant, stretch;
    // Stop search at `sizeFamily.index`
    auto string = std::string(str);
    auto substr = string.substr(0, sizeFamily.position()).c_str();
    if (std::regex_match(substr, weight, WEIGHT_REG))
        font->weight = weight[1].str().c_str();
    if (std::regex_match(substr, style, STYLE_REG))
        font->style = style[1].str().c_str();
    if (std::regex_match(substr, variant, VARIANT_REG))
        font->variant = variant[1].str().c_str();
    if (std::regex_match(substr, stretch, STRETCH_REG))
        font->stretch = stretch[1].str().c_str();

    if (font->unit == "pt") {
        font->size /= 0.75;
    } else if (font->unit == "pc") {
        font->size *= 16;
    } else if (font->unit == "in") {
        font->size *= 96;
    } else if (font->unit == "cm") {
        font->size *= 96.0 / 2.54;
    } else if (font->unit == "mm") {
        font->size *= 96.0 / 25.4;
    } else if (font->unit == "%") {
        // TODO disabled because existing unit tests assume 100
        // font->size *= defaultHeight / 100 / 0.75
    } else if (font->unit == "em" || font->unit == "rem") {
        font->size *= defaultHeight / 0.75;
    } else if (font->unit == "q") {
        font->size *= 96 / 25.4 / 4;
    }

    cache[str] = font;

    return font;
};
