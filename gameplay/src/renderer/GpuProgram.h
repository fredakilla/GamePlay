#ifndef GPUPROGRAM_H
#define GPUPROGRAM_H

#include "Base.h"

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
    ST_VERTEX,
    ST_FRAGMENT,
    ST_COMPUTE
};

struct GpuProgramHandle
{
};

class GpuProgram
{
public:
    virtual ~GpuProgram() {}
    virtual void set(ShaderFiles shaderFiles) = 0;
    virtual void bind() = 0;

    //virtual GpuProgramHandle getHandle();

protected:
    ShaderFiles _shaderFiles;

};

}

#endif // GPUPROGRAM_H
