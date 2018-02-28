#pragma once

#include "../Base.h"
#include "../BGFX/BGFXUniform.h"

namespace gameplay
{

class BGFXGpuProgram
{
public:
    BGFXGpuProgram();
    ~BGFXGpuProgram();
    void set(const char* vshPath, const char* fshPath, const char* defines);
    void bind();
    const bgfx::ProgramHandle getProgram() const;
    const std::vector<UniformInfo> getUniformsInfo() const { return _uniformsInfo; }

protected:
    std::vector<UniformInfo> _uniformsInfo;

private:
    void getUniformsFromShader(bgfx::ShaderHandle shaderHandle);

    bgfx::ShaderHandle _vsh;
    bgfx::ShaderHandle _fsh;
    bgfx::ShaderHandle _csh;
    bgfx::ProgramHandle _program;
};

}

