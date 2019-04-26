#include "translator.h"

namespace BKShaderTranslator {
    const unsigned int NUM_SOURCE_STRINGS = 1;

    std::string translate(GLenum type, const std::string& str) {

        ShShaderSpec spec = SH_WEBGL_SPEC;
        ShShaderOutput output = SH_GLSL_330_CORE_OUTPUT;

        sh::Initialize();

        ShBuiltInResources resources;
        _getResources(&resources);

        sh::Initialize();

        auto compiler = sh::ConstructCompiler(type, spec, output, &resources);

        auto x = str.c_str();

        std::string code = "";
        int ret = sh::Compile(compiler, &x, 1, SH_VALIDATE | SH_OBJECT_CODE);
        if (ret) {
            code = sh::GetObjectCode(compiler);
            printf("output\n");
            printf(code.c_str());
            printf("\n\n");
        }

        sh::Destruct(compiler);
        sh::Finalize();
        return code;
    }

    void _getResources(ShBuiltInResources* resources) {
        sh::InitBuiltInResources(resources);

        resources->MaxVertexAttribs = 8;
        resources->MaxVertexUniformVectors = 128;
        resources->MaxVaryingVectors = 8;
        resources->MaxVertexTextureImageUnits = 0;
        resources->MaxCombinedTextureImageUnits = 8;
        resources->MaxTextureImageUnits = 8;
        resources->MaxFragmentUniformVectors = 16;
        resources->MaxDrawBuffers = 1;
        resources->MaxDualSourceDrawBuffers = 1;

        resources->OES_standard_derivatives = 0;
        resources->OES_EGL_image_external = 0;
        resources->EXT_geometry_shader = 1;
        resources->ANGLE_texture_multisample = 0;
    }
}  // namespace BKShaderTranslator