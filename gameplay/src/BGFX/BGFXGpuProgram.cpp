#include "../BGFX/BGFXGpuProgram.h"
#include "../FileSystem.h"

#define GP_BGFX_RTSHADERC

#ifdef GP_BGFX_RTSHADERC
#include <bgfx/tools/shaderc/shaderc.h>
namespace bgfx
{
    int compileShader(int _argc, const char* _argv[]);
}
#endif

namespace gameplay {

BGFXGpuProgram::BGFXGpuProgram()
{
    _vsh = BGFX_INVALID_HANDLE;
    _fsh = BGFX_INVALID_HANDLE;
    _csh = BGFX_INVALID_HANDLE;
    _program = BGFX_INVALID_HANDLE;
}

BGFXGpuProgram::~BGFXGpuProgram()
{
    if(bgfx::isValid(_program))
        bgfx::destroy(_program);
    if(bgfx::isValid(_vsh))
        bgfx::destroy(_vsh);
    if(bgfx::isValid(_fsh))
        bgfx::destroy(_fsh);
    if(bgfx::isValid(_csh))
        bgfx::destroy(_csh);
}

bool detectShadercSignature(std::string filename)
{
    // Open the file.
    std::unique_ptr<Stream> stream(FileSystem::open(filename.c_str()));
    if (stream.get() == NULL || !stream->canRead())
    {
        GP_ERROR("Failed to open shader file '%s'.", filename.c_str());
        return false;
    }

    // Verify shaderc signature.
    char sig[3];
    if ( stream->read(sig, 1, 3) == 3
         && (sig[0] == 'V' && sig[1] == 'S' && sig[2] == 'H')
         || (sig[0] == 'F' && sig[1] == 'S' && sig[2] == 'H')
       )
    {
        // shaderc signature detected.
        return true;
    }

    // shaderc signature not detected.
    return false;
}

unsigned long hash_djb2(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

std::string compileShader(std::string shaderFile, std::string defines)
{
    // get shader file upper extension (.VS or .FS)
    std::string extension = FileSystem::getExtension(shaderFile.c_str());

    // varyingDef name is same that shader filename but with .io extension
    std::string varyingDef = FileSystem::getBaseName(shaderFile.c_str()) + ".io";

    // get shader basename without path and without extension
    std::string base_filename = shaderFile.substr(shaderFile.find_last_of("/\\") + 1);
    std::string::size_type const p(base_filename.find_last_of('.'));
    std::string file_without_extension = base_filename.substr(0, p);

    // format ouput compiled shader filename using basename + hash key based on defines
    std::string outFileName = "tmpshaders/" + file_without_extension;  
    outFileName += "_";
    outFileName += std::to_string(hash_djb2(defines.c_str()));

    // add to ouput name, shader type extension
    unsigned char type = 255;
    if(extension == ".VERT")
    {
        type = 2;
        outFileName += ".vs";
    }
    else if(extension == ".FRAG")
    {
        type = 1;
        outFileName += ".fs";
    }
    else
    {
        // not valid extension
        return std::string("");
    }
    GP_ASSERT(type != 255);


    // shaderc --type : shader type (v = vertex, f = fragment, c = compute)
    static const char ST[9][2] = {"", "f", "v", "", "g", "", "", "", "c"};

    // shaderc --profile : shader model (f.e. ps_3_0)
    static const char SM[9][7] = {"", "ps_5_0", "vs_5_0", "", "gs_5_0", "", "", "", "cs_5_0"};

    // build shaderc command line
    // ex: ./shaderc -f colored.fs -o colored_VERTEX_COLOR.frag --varyingdef colored.io --type f --platform linux --define VERTEX_COLOR
    int argc = 0;
    const char* argv[16];
    argv[argc++] = "-f";
    argv[argc++] = shaderFile.c_str();
    argv[argc++] = "-o";
    argv[argc++] = outFileName.c_str();
    argv[argc++] = "--varyingdef";
    argv[argc++] = "res/shaders/varying.def.sc";//varyingDef.c_str();
    argv[argc++] = "--type";
    argv[argc++] = ST[type];
    argv[argc++] = "--platform";
    argv[argc++] = "linux";
    //argv[argc++] = "--profile";
    //argv[argc++] = SM[shader->type]; //"vs_5_0";
    //argv[argc++] = "--raw";
    argv[argc++] = "--define";
    argv[argc++] = defines.c_str();

    // invoke shaderc compiler (static library version)
    int ret = bgfx::compileShader(argc, argv);
    GP_ASSERT(ret == 0);

    // log shaderc build command to a file (for compile time usage).
    std::string commandLineStr= "$SHADERC ";
    for (int i=0;i<argc;i++) commandLineStr.append(argv[i]).append(" ");
    Stream * stream = FileSystem::open("tmpshaders/build-shader.txt", FileSystem::WRITE);
    stream->write(commandLineStr.c_str(), 1, commandLineStr.size());

    // return ouput compiled shader filename
    return outFileName;
}


void BGFXGpuProgram::set(ShaderFiles shaderFiles)
{
    // try to detect shaderc signature in shader files, if not, try to compile shader.

    if(!detectShadercSignature(shaderFiles.vertex))
    {
         std::string vertFile = compileShader(shaderFiles.vertex, shaderFiles.defines);
         if(!vertFile.empty())
             shaderFiles.vertex = vertFile;
    }

    if(!detectShadercSignature(shaderFiles.fragment))
    {
         std::string fragFile = compileShader(shaderFiles.fragment, shaderFiles.defines);
         if(!fragFile.empty())
             shaderFiles.fragment = fragFile;
    }

    /*// try shaderc compilation
    // if fails, presume shader is already compiled (extension .vert or .frag) and fallback to shader creation
    std::string vertFile = compileShader(shaderFiles.vertex, shaderFiles.defines);
    std::string fragFile = compileShader(shaderFiles.fragment, shaderFiles.defines);
    if(!vertFile.empty() && !fragFile.empty())
    {
        // shaderc compilation successfull.
        shaderFiles.vertex = vertFile;
        shaderFiles.fragment = fragFile;
    }*/



    // Create bgfx shaders from compiled shaderc output files.
    createShader(shaderFiles.vertex.c_str(), _vsh);
    createShader(shaderFiles.fragment.c_str(), _fsh);

    // Create bgfx program.
    _program = bgfx::createProgram(_vsh,_fsh,false);
    GP_ASSERT(bgfx::isValid(_program));

    // Query uniforms from shaders.
    getUniformsFromShader(_vsh);
    getUniformsFromShader(_fsh);
}

void BGFXGpuProgram::getUniformsFromShader(bgfx::ShaderHandle shaderHandle)
{
    bgfx::UniformHandle uniforms[32];
    uint16_t activeUniforms = bgfx::getShaderUniforms(shaderHandle, &uniforms[0], 32);    

    for (int i = 0; i < activeUniforms; ++i)
    {
        bgfx::UniformInfo info;
        bgfx::getUniformInfo(uniforms[i], info);

        gameplay::UniformInfo uinfo;
        uinfo.name = info.name;
        uinfo.num = info.num;
        switch(info.type)
        {
            case bgfx::UniformType::Int1: // Int, used for samplers only.
                uinfo.type = UniformType::UT_SAMPLER;
                break;
            case bgfx::UniformType::Vec4: // 4 floats vector.
                uinfo.type = UniformType::UT_VECTOR4;
                break;
            case bgfx::UniformType::Mat3: // 3x3 matrix.
                uinfo.type = UniformType::UT_MATRIX3;
                break;
            case bgfx::UniformType::Mat4: // 4x4 matrix.
                uinfo.type = UniformType::UT_MATRIX4;
                break;
            default:
                GP_ERROR("BGFXGpuProgram::addUniformsFromShader: Uniform type unknown.");
        };

        _uniformsInfo.push_back(uinfo);        
    }
}

void BGFXGpuProgram::createShader(const char * binFile, bgfx::ShaderHandle& shader)
{
    int shSize = 0;
    char * shSource = FileSystem::readAll(binFile, &shSize);
    if (shSource == NULL)
    {
        GP_ERROR("Failed to read shader from file '%s'.", binFile);
        return;
    }

    const bgfx::Memory* mem = bgfx::alloc(shSize + 1);
    memcpy((void *)mem->data, (void *)shSource, shSize);
    delete shSource;

    mem->data[mem->size - 1] = '\0';

    shader = bgfx::createShader(mem);
    GP_ASSERT(bgfx::isValid(shader));
}

void BGFXGpuProgram::bind()
{
}

const bgfx::ProgramHandle BGFXGpuProgram::getProgram() const
{
    return _program;
}

} // end namespace gameplay

