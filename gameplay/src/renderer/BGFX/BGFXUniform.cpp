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


void BGFXUniform::setValue(Uniform* uniform, float value)
{
    BGFXUniform * bgfxUniform = static_cast<BGFXUniform*>(uniform);
    GP_ASSERT(bgfxUniform && bgfx::isValid(bgfxUniform->_handle));
    bgfx::setUniform(bgfxUniform->_handle, &value);
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


void BGFXUniform::setValue(Uniform* uniform, const Matrix* values, unsigned int count)
{
    BGFXUniform * bgfxUniform = static_cast<BGFXUniform*>(uniform);
    GP_ASSERT(bgfxUniform && bgfx::isValid(bgfxUniform->_handle));
    bgfx::setUniform(bgfxUniform->_handle, &values[0].m, count);
}

void BGFXUniform::setValue(Uniform* uniform, const Matrix& value)
{
    BGFXUniform * bgfxUniform = static_cast<BGFXUniform*>(uniform);
    GP_ASSERT(bgfxUniform && bgfx::isValid(bgfxUniform->_handle));
    bgfx::setUniform(bgfxUniform->_handle, &value.m);
}



void BGFXUniform::setValue(Uniform* uniform, const Texture::Sampler* sampler)
{
    GP_ASSERT(uniform);
    GP_ASSERT(uniform->getType() == UT_SAMPLER);
    GP_ASSERT(sampler);

    // Bind the sampler - this binds the texture and applies sampler state
    const_cast<Texture::Sampler*>(sampler)->bind(uniform);



    /*GP_ASSERT(uniform);
    GP_ASSERT(uniform->_type == GL_SAMPLER_2D || uniform->_type == GL_SAMPLER_CUBE);
    GP_ASSERT(sampler);
    GP_ASSERT((sampler->getTexture()->getType() == Texture::TEXTURE_2D && uniform->_type == GL_SAMPLER_2D) ||
        (sampler->getTexture()->getType() == Texture::TEXTURE_CUBE && uniform->_type == GL_SAMPLER_CUBE));

    GL_ASSERT( glActiveTexture(GL_TEXTURE0 + uniform->_index) );

    // Bind the sampler - this binds the texture and applies sampler state
    const_cast<Texture::Sampler*>(sampler)->bind();

    GL_ASSERT( glUniform1i(uniform->_location, uniform->_index) );*/
}



} // end namespace gameplay
