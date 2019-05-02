#if BK_ENABLE_SHADER_TRANSLATOR

#include "translator.h"

#ifdef WIN32
#pragma comment(lib, "angle_common")
#pragma comment(lib, "preprocessor")
#pragma comment(lib, "translator")
#endif

namespace BKShaderTranslator {

    ShHandle vertextCompiler;
    ShHandle fragmentCompiler;

    void initialize() {
        sh::Initialize();

        ShShaderSpec spec = SH_WEBGL_SPEC;
        ShShaderOutput output = SH_GLSL_330_CORE_OUTPUT;

        ShBuiltInResources resources;
        sh::InitBuiltInResources(&resources);

        // resources.MaxVertexAttribs = 16;
        // resources.MaxVertexUniformVectors = 1024;
        // resources.MaxVaryingVectors = 8;
        // resources.MaxVertexTextureImageUnits = 0;
        // resources.MaxCombinedTextureImageUnits = 8;
        // resources.MaxTextureImageUnits = 8;
        // resources.MaxFragmentUniformVectors = 16;
        // resources.MaxDrawBuffers = 1;
        // resources.MaxDualSourceDrawBuffers = 1;
        resources.OES_standard_derivatives = 1;
        resources.OES_EGL_image_external = 1;
        resources.EXT_geometry_shader = 0;
        resources.ANGLE_texture_multisample = 0;

        vertextCompiler = sh::ConstructCompiler(GL_VERTEX_SHADER, spec, output, &resources);
        fragmentCompiler = sh::ConstructCompiler(GL_FRAGMENT_SHADER, spec, output, &resources);
    }

    void finalize() {
        sh::Finalize();
    }

    bool translate(GLenum type, const std::string& str, std::string& output) {

        ShHandle compiler;
        if (type == GL_VERTEX_SHADER) {
            compiler = vertextCompiler;
        } else if (type == GL_FRAGMENT_SHADER) {
            compiler = fragmentCompiler;
        } else {
            printf("[Shader Translator] unsupported shader type.\n");
        }

        auto x = str.c_str();

        int ret = sh::Compile(compiler, &x, 1, SH_VALIDATE | SH_OBJECT_CODE);
        if (ret) {
            output = sh::GetObjectCode(compiler);
        } else {
            printf("[Shader Translator] error occurs while compiling.\n");
        }

        return ret;
    }
}  // namespace BKShaderTranslator

#endif