#include "BGFXUniform.h"
#include "Vector4.h"

namespace gameplay {

BGFXUniform::BGFXUniform(const char * name, UniformType type, uint16_t num) :
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
            GP_ERROR("BGFXGpuProgram: Uniform type unknown.");
    };

    _handle = bgfx::createUniform(name, bgfxUniformType, num);
    GP_ASSERT(bgfx::isValid(_handle));
}

BGFXUniform::~BGFXUniform()
{
    if(bgfx::isValid(_handle))
    {
         bgfx::destroy(_handle);
    }
}


void BGFXUniform::setValue(Uniform* uniform, const Vector4* values, unsigned int count)
{
    BGFXUniform * bgfxUniform = static_cast<BGFXUniform*>(uniform);
    GP_ASSERT(bgfxUniform && bgfx::isValid(bgfxUniform->_handle));
    bgfx::setUniform(bgfxUniform->_handle, &values[0].x, count);
}


void BGFXUniform::setValue(Uniform* uniform, const Vector4& value)
{
    BGFXUniform * bgfxUniform = static_cast<BGFXUniform*>(uniform);
    GP_ASSERT(bgfxUniform && bgfx::isValid(bgfxUniform->_handle));
    bgfx::setUniform(bgfxUniform->_handle, &value.x);
}


} // end namespace gameplay
