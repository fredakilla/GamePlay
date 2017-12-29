#ifndef BGFXGPUPROGRAM_H
#define BGFXGPUPROGRAM_H

#include "GpuProgram.h"
#include "Matrix.h"
#include "Uniform.h"

namespace gameplay
{


class BGFXUniform : public Uniform
{

public:


    BGFXUniform(const char * name, UniformType type, uint16_t num) :
        _handle(BGFX_INVALID_HANDLE)
    {

        bgfx::UniformType::Enum bgfxUniformType;
        switch(type)
        {
            case UniformType::UT_SAMPLER: //!< Int, used for samplers only.
                bgfxUniformType =  bgfx::UniformType::Int1;
                break;
            case UniformType::UT_VECTOR4: //!< 4 floats vector.
                bgfxUniformType = bgfx::UniformType::Vec4;
                break;
            case UniformType::UT_MATRIX3: //!< 3x3 matrix.
                bgfxUniformType = bgfx::UniformType::Mat3;
                break;
            case UniformType::UT_MATRIX4: //!< 4x4 matrix.
                bgfxUniformType = bgfx::UniformType::Mat4;
                break;
            default:
                GP_ERROR("BGFXGpuProgram::addUniformsFromShader: Uniform type unknown.");
        };

        _handle = bgfx::createUniform(name, bgfxUniformType, num);
        GP_ASSERT(bgfx::isValid(_handle));
    }

    ~BGFXUniform()
    {
        if(bgfx::isValid(_handle))
        {
             bgfx::destroy(_handle);
        }
    }










    void setValue(Uniform* uniform, const Vector3& value)
    {
        BGFXUniform * bgfxUniform = static_cast<BGFXUniform*>(uniform);
        GP_ASSERT(bgfxUniform && bgfx::isValid(bgfxUniform->_handle));

        //bgfx::setUniform(bgfxUniform->_handle, value, 3);
    }



    void setValue(Uniform* uniform, const Vector4* values, unsigned int count = 1)
    {
        BGFXUniform * bgfxUniform = static_cast<BGFXUniform*>(uniform);
        GP_ASSERT(bgfxUniform && bgfx::isValid(bgfxUniform->_handle));
        bgfx::setUniform(bgfxUniform->_handle, &values[0].x, count);
    }


    void setValue(Uniform* uniform, const Vector4& value)
    {
        BGFXUniform * bgfxUniform = static_cast<BGFXUniform*>(uniform);
        GP_ASSERT(bgfxUniform && bgfx::isValid(bgfxUniform->_handle));
        bgfx::setUniform(bgfxUniform->_handle, &value.x);
    }















private:
    bgfx::UniformHandle _handle;
};




class BGFXGpuProgram : public GpuProgram
{
public:

    BGFXGpuProgram();
    ~BGFXGpuProgram();
    virtual void set(ShaderFiles shaderFiles) override;
    void bind() override;


    const bgfx::ProgramHandle getProgram() const;


private:

    void getUniformsFromShader(bgfx::ShaderHandle shaderHandle);
    void createShader(const char * binFile, bgfx::ShaderHandle& shader);

    void compilShaderFromSource(ShaderType type, const char *src, const char *define, bgfx::ShaderHandle &shader, const char *varyingDef);

    bgfx::ShaderHandle _vsh;
    bgfx::ShaderHandle _fsh;
    bgfx::ShaderHandle _csh;
    bgfx::ProgramHandle _program;

};

}

#endif // BGFXGPUPROGRAM_H
