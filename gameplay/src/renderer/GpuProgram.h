#ifndef GPUPROGRAM_H
#define GPUPROGRAM_H

#include "Base.h"
#include "Uniform.h"

namespace gameplay
{

class Matrix;

//------------------------------------------------------------------------------
// Shaders
//------------------------------------------------------------------------------

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




//------------------------------------------------------------------------------
// GPU Program
//------------------------------------------------------------------------------

class GpuProgram
{
public:
    virtual ~GpuProgram() {}
    virtual void set(ShaderFiles shaderFiles) = 0;
    virtual void bind() = 0;

    const std::vector<UniformInfo> getUniformsInfo() const { return _uniformsInfo; }


    //virtual void setUniform(Uniform * uniform, const Matrix* values, unsigned int count) = 0;


protected:
    ShaderFiles _shaderFiles;

    std::vector<UniformInfo> _uniformsInfo;

};

}

#endif // GPUPROGRAM_H
