#ifndef GPUPROGRAM_H
#define GPUPROGRAM_H

#include "Base.h"

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
// Uniforms
//------------------------------------------------------------------------------

enum UniformType
{
    UT_SAMPLER,             /// Int, used for samplers only.
    UT_VECTOR4,             /// 4 floats vector.
    UT_MATRIX3,             /// 3x3 matrix.
    UT_MATRIX4,             /// 4x4 matrix.
};

struct UniformInfo
{
    std::string name;       /// Uniform name.
    UniformType type;       /// Uniform type.
    unsigned short num;     /// Number of elements in array.
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
