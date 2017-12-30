#ifndef BGFXUNIFORM_H
#define BGFXUNIFORM_H

#include "Base.h"
#include "Uniform.h"


namespace gameplay
{

class Vector4;

class BGFXUniform : public Uniform
{

public:


    BGFXUniform(const char * name, UniformType type, uint16_t num);
    ~BGFXUniform();

    void setValue(Uniform* uniform, const Vector4* values, unsigned int count = 1) override;
    void setValue(Uniform* uniform, const Vector4& value) override;
    void setValue(Uniform* uniform, const Texture::Sampler* sampler) override;

    bgfx::UniformHandle getHandle() { return _handle; }


private:
    bgfx::UniformHandle _handle;
};

} // end namespace gameplay

#endif // BGFXUNIFORM_H
