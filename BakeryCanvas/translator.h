#include <GLSLANG/ShaderLang.h>

// #include <angle_gl.h>
#include <glad/glad.h>
#include <assert.h>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace BKShaderTranslator {
    std::string translate(const std::string& str, GLenum type);
    void GenerateResources(ShBuiltInResources* resources);
    static bool ReadShaderSource(const char* fileName, std::string &source);
}