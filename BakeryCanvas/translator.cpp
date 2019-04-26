#include "translator.h"

#ifdef WIN32
#pragma comment(lib, "angle_common")
#pragma comment(lib, "preprocessor")
#pragma comment(lib, "translator")
#endif

namespace BKShaderTranslator {
    const unsigned int NUM_SOURCE_STRINGS = 1;

    std::string translate(const std::string& str, GLenum type) {

        ShShaderSpec spec = SH_WEBGL_SPEC;
        ShShaderOutput output = SH_GLSL_330_CORE_OUTPUT;

        sh::Initialize();

        ShBuiltInResources resources;
        GenerateResources(&resources);

        sh::Initialize();
        auto compiler = sh::ConstructCompiler(type, spec, output, &resources);

  //      std::string source;
		//if (!ReadShaderSource("/Users/Icemic/Workspace/BakeryCanvas/test/fixtures/test.frag", source))
		//{
		//	source = "attribute vec3 aPos;"  "void main(void){ gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);" "}";
		//}

        // const char* const shaderStrings[] = { "attribute vec3 aPos;"
        //                                       "void main(void){ gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        //                                       "}" };

        // std::string str1 = "attribute vec3 aPos;void main(void){ gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}";
        // auto str2 = str1.c_str();
        // auto str3 = &str2;
		auto v = str.c_str();
        int ret = sh::Compile(compiler, &v, 1, SH_VALIDATE | SH_OBJECT_CODE);
        printf("%d\n", ret);
        if (ret) {
            std::string code = sh::GetObjectCode(compiler);
            printf(code.c_str());
            printf("\n\n");
            return code;
        }
		return str;
    }

    static bool ReadShaderSource(const char* fileName, std::string &source) {
        FILE* in = fopen(fileName, "rb");
        if (!in) {
            printf("Error: unable to open input file: %s\n", fileName);
            return false;
        }

        // Obtain file size.
        fseek(in, 0, SEEK_END);
        size_t count = ftell(in);
		fseek(in, 0, SEEK_SET);
		char *buf = (char*)malloc(count + 1);
		fread(buf, 1, count, in);
		buf[count] = 0;
		source = buf;

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