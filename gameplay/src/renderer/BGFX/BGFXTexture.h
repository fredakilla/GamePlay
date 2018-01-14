#ifndef BGFXTEXTUREHANDLE_H
#define BGFXTEXTUREHANDLE_H

#include "../../renderer/TextureHandle.h"
#include "../../graphics/Texture.h"

namespace gameplay {

class BGFXTexture : public GpuTexture
{
public:
    BGFXTexture(Texture *texture, const unsigned char* data, unsigned int size, Texture::Type type);
    ~BGFXTexture();

    void bind(Uniform * uniform) override;

    const bgfx::TextureHandle getHandle() const { return _handle; }

private:
    bgfx::TextureHandle _handle;
    Texture* _texture;

    uint32_t _flags;

};

} // end namespace gameplay

#endif // BGFXTEXTUREHANDLE_H
