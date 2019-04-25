#include "translator.h"

namespace BKShaderTranslator {
    const unsigned int NUM_SOURCE_STRINGS = 1;

    std::string translate(std::string& str) {

        ShShaderSpec spec = SH_WEBGL_SPEC;
        ShShaderOutput output = SH_GLSL_330_CORE_OUTPUT;

        sh::Initialize();

        ShBuiltInResources resources;
        GenerateResources(&resources);

        sh::Initialize();
        auto compiler = sh::ConstructCompiler(GL_FRAGMENT_SHADER, spec, output, &resources);

        ShaderSource source;
        if (!ReadShaderSource("/Users/Icemic/Workspace/BakeryCanvas/test/fixtures/test.frag", source))
            return "";

        // const char* const shaderStrings[] = { "attribute vec3 aPos;"
        //                                       "void main(void){ gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        //                                       "}" };

        // std::string str1 = "attribute vec3 aPos;void main(void){ gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}";
        // auto str2 = str1.c_str();
        // auto str3 = &str2;

        int ret = sh::Compile(compiler, &source[0], source.size(), SH_VALIDATE | SH_OBJECT_CODE);
        printf("%d", ret);
        if (ret) {
            std::string code = sh::GetObjectCode(compiler);
            printf(code.c_str());
            printf("\n\n");
            return code;
        }
        return "";
    }

    static bool ReadShaderSource(const char* fileName, ShaderSource& source) {
        FILE* in = fopen(fileName, "rb");
        if (!in) {
            printf("Error: unable to open input file: %s\n", fileName);
            return false;
        }

        // Obtain file size.
        fseek(in, 0, SEEK_END);
        size_t count = ftell(in);
        rewind(in);

        int len = (int)ceil((float)count / (float)NUM_SOURCE_STRINGS);
        source.reserve(NUM_SOURCE_STRINGS);
        // Notice the usage of do-while instead of a while loop here.
        // It is there to handle empty files in which case a single empty
        // string is added to vector.
        do {
            char* data = new char[len + 1];
            size_t nread = fread(data, 1, len, in);
            data[nread] = '\0';
            source.push_back(data);

            count -= nread;
        } while (count > 0);

        fclose(in);
        return true;
    }

    void GenerateResources(ShBuiltInResources* resources) {
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