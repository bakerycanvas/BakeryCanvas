#if defined(max)
#undef max
#endif

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
    std::string translate(GLenum type, const std::string& str);
    void _getResources(ShBuiltInResources* resources);
}