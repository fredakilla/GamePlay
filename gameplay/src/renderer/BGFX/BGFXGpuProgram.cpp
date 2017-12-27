#include "BGFXGpuProgram.h"
#include "FileSystem.h"

#include <shaderc.h>

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

}

void BGFXGpuProgram::set(ShaderFiles shaderFiles)
{
    _shaderFiles = shaderFiles;

    // Create bgfx shaders from bin files.
    createShader(shaderFiles.vertex.c_str(), _vsh);
    createShader(shaderFiles.fragment.c_str(), _fsh);

    // Create bgfx program.
    _program = bgfx::createProgram(_vsh,_fsh,false);
    GP_ASSERT(bgfx::isValid(_program));

    // Query uniforms from shaders.
    //addUniformsFromShader(vsh);
    //addUniformsFromShader(fsh);


    // Set varyingDef filename based on vertex basename.
    //std::string basename = FileSystem::getBaseName(shaderFiles.vertex.c_str());
    //std::string varyingDef = basename.append(".io");

    // compil using shaderc lib
    // compilShader(ST_VERTEX, shaderFiles.vertex.c_str(), shaderFiles.defines.c_str(), vsh, varyingDef.c_str());
}

void BGFXGpuProgram::addUniformsFromShader(bgfx::ShaderHandle shaderHandle)
{
    /*bgfx::UniformHandle uniforms[16];
    uint16_t activeUniforms = bgfx::getShaderUniforms(shaderHandle, &uniforms[0], 16);

    for (int i = 0; i < activeUniforms; ++i)
    {
        unsigned int samplerIndex = 0;

        bgfx::UniformInfo info;
        bgfx::getUniformInfo(uniforms[0], info);

        Uniform* uniform = new Uniform();
        uniform->_effect = this;
        uniform->_name = info.name;
        //uniform->_location = uniformLocation;
        uniform->_type = info.type;
        if (info.type == Uniform::UNIFORM_SAMPLER)
        {
            uniform->_index = samplerIndex;
            //samplerIndex += uniformSize;
        }
        else
        {
            uniform->_index = 0;
        }

        _uniforms[info.name] = uniform;
    }*/
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

void BGFXGpuProgram::compilShaderFromSource(ShaderType type, const char *src, const char *define, bgfx::ShaderHandle& shader, const char * varyingDef)
{
    std::string fullname(src);
    std::string rawname = fullname.substr(0, fullname.find_last_of(".")).c_str();
    std::string path = fullname.substr(0, fullname.find_last_of("\\/")).c_str();


    std::string binFile(rawname + ".bin");
    //eString varyingDefFile(path + varyingDef); //"/varying.def.sc");

    // use shaderc to generate bgfx shader bin file

    // shaderc --type : Shader type (v = vertex, f = fragment)
    //static const char ST[9][2] = {"", "f", "v", "", "g", "", "", "", "c"};
    static const char ST[9][2] = { "v", "f", "c" };

    // shaderc --profile : Shader model (f.e. ps_3_0)
    static const char SM[9][7] = {"", "ps_5_0", "vs_5_0", "", "gs_5_0", "", "", "", "cs_5_0"};


#ifdef WIN32
    const char * platformStr = "windows";
#elif __linux__
    const char * platformStr = "linux";
#else
    // undefined platform
    GP_ASSERT(0);
    const char * platformStr = "Unknown";
#endif


    int argc = 0;
    const char* argv[16];

    argv[argc++] = "-f";
    argv[argc++] = src;
    argv[argc++] = "-o";
    argv[argc++] = binFile.c_str();
    argv[argc++] = "--varyingdef";
    argv[argc++] = varyingDef; //varyingDefFile;
    argv[argc++] = "--type";
    argv[argc++] = ST[type];
    argv[argc++] = "--platform";
    argv[argc++] = platformStr;
    //argv[argc++] = "--profile";
    //argv[argc++] = SM[shader->type]; //"vs_5_0";
    //argv[argc++] = "--raw";

    char outputText[2048];
    uint16_t outputSize;

    int ret = bgfx::compileShader(argc, argv);
    bgfx::getShaderError(outputText, outputSize);

    if(ret)
    {
        GP_ERROR(outputText);
        return;
    }


    // create bgfx shader from .bin

    int shSize = 0;
    char * shSource = FileSystem::readAll(src, &shSize);
    if (shSource == NULL)
    {
        GP_ERROR("Failed to read shader from file '%s'.", src);
        return;
    }

    const bgfx::Memory* mem = bgfx::alloc(shSize + 1);
    memcpy((void *)mem->data, (void *)shSource, shSize);
    delete shSource;

    mem->data[mem->size - 1] = '\0';

    shader = bgfx::createShader(mem);
    GP_ASSERT(bgfx::isValid(shader));
}



} // end namespace gameplay

