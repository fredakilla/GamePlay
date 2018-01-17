#include "../../renderer/BGFX/BGFXGpuProgram.h"
#include "../../core/FileSystem.h"

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

