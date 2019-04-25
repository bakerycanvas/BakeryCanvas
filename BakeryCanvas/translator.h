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
    typedef std::vector<char*> ShaderSource;
    std::string translate(std::string& str);
    void GenerateResources(ShBuiltInResources* resources);
    static bool ReadShaderSource(const char* fileName, ShaderSource& source);
}