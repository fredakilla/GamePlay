#ifndef BGFXGPUPROGRAM_H
#define BGFXGPUPROGRAM_H

#include "../Base.h"
#include "../BGFX/BGFXUniform.h"

namespace gameplay
{


struct ShaderFiles
{
    std::string vertex;
    std::string fragment;
    std::string defines;
};

enum ShaderType
{
    ST_VERTEX,              /// Vertex Shader.
    ST_FRAGMENT,            /// Fragment Shader.
    ST_COMPUTE              /// Compute Shader.
};


class BGFXGpuProgram
{
public:
    BGFXGpuProgram();
    ~BGFXGpuProgram();
    virtual void set(ShaderFiles shaderFiles);
    void bind();
    const bgfx::ProgramHandle getProgram() const;
    const std::vector<UniformInfo> getUniformsInfo() const { return _uniformsInfo; }

protected:
    std::vector<UniformInfo> _uniformsInfo;

private:
    void getUniformsFromShader(bgfx::ShaderHandle shaderHandle);
    void createShader(const char * binFile, bgfx::ShaderHandle& shader);

    bgfx::ShaderHandle _vsh;
    bgfx::ShaderHandle _fsh;
    bgfx::ShaderHandle _csh;
    bgfx::ProgramHandle _program;
};

} // end namespace gameplay

#endif // BGFXGPUPROGRAM_H
