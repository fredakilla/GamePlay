#ifndef BGFXGPUPROGRAM_H
#define BGFXGPUPROGRAM_H

#include "GpuProgram.h"

namespace gameplay
{

class BGFXGpuProgram : public GpuProgram
{
public:
    BGFXGpuProgram();
    ~BGFXGpuProgram();
    virtual void set(ShaderFiles shaderFiles) override;
    //virtual void bind() override;

private:

    void addUniformsFromShader(bgfx::ShaderHandle shaderHandle);
    void createShader(const char * binFile, bgfx::ShaderHandle& shader);

    void compilShaderFromSource(ShaderType type, const char *src, const char *define, bgfx::ShaderHandle &shader, const char *varyingDef);

    bgfx::ShaderHandle _vsh;
    bgfx::ShaderHandle _fsh;
    bgfx::ShaderHandle _csh;
    bgfx::ProgramHandle _program;

};

}

#endif // BGFXGPUPROGRAM_H
