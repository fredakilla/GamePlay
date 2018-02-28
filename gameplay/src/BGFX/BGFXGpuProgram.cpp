#include "../BGFX/BGFXGpuProgram.h"
#include "../FileSystem.h"

#include <brtshaderc/brtshaderc.h>


namespace gameplay {

BGFXGpuProgram::BGFXGpuProgram() :
    _vsh(BGFX_INVALID_HANDLE)
    , _fsh(BGFX_INVALID_HANDLE)
    , _csh(BGFX_INVALID_HANDLE)
    , _program(BGFX_INVALID_HANDLE)
{

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

void BGFXGpuProgram::set(const char* vshPath, const char* fshPath, const char* defines)
{
    // Compile shaders using brtshaderc library
    const bgfx::Memory* memVsh = shaderc::compileShader(shaderc::ST_VERTEX, vshPath, defines, "res/shaders/varying.def.sc");
    const bgfx::Memory* memFsh = shaderc::compileShader(shaderc::ST_FRAGMENT, fshPath, defines, "res/shaders/varying.def.sc");

    GP_ASSERT(memVsh);
    GP_ASSERT(memFsh);

    // Create shaders.
    _vsh = bgfx::createShader(memVsh);
    _fsh = bgfx::createShader(memFsh);

    // Create bgfx program.
    _program = bgfx::createProgram(_vsh, _fsh, true);
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
                GP_ERROR("Uniform type is unknown.");
        };

        _uniformsInfo.push_back(uinfo);        
    }
}

void BGFXGpuProgram::bind()
{
}

const bgfx::ProgramHandle BGFXGpuProgram::getProgram() const
{
    return _program;
}

} // end namespace gameplay

