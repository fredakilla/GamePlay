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

    void setValue(Uniform* uniform, float value) override;
    void setValue(Uniform* uniform, const float* values, unsigned int count = 1) override;
    void setValue(Uniform* uniform, int value) override;
    void setValue(Uniform* uniform, const int* values, unsigned int count = 1) override;
    void setValue(Uniform* uniform, const Matrix& value) override;
    void setValue(Uniform* uniform, const Matrix* values, unsigned int count = 1) override;
    void setValue(Uniform* uniform, const Vector2& value) override;
    void setValue(Uniform* uniform, const Vector2* values, unsigned int count = 1) override;
    void setValue(Uniform* uniform, const Vector3& value) override;
    void setValue(Uniform* uniform, const Vector3* values, unsigned int count = 1) override;
    void setValue(Uniform* uniform, const Vector4& value) override;
    void setValue(Uniform* uniform, const Vector4* values, unsigned int count = 1) override;
    void setValue(Uniform* uniform, const Texture::Sampler* sampler) override;
    //void setValue(Uniform* uniform, const Texture::Sampler** values, unsigned int count) override;

    bgfx::UniformHandle getHandle() { return _handle; }


private:
    bgfx::UniformHandle _handle;
};

} // end namespace gameplay

#endif // BGFXUNIFORM_H
