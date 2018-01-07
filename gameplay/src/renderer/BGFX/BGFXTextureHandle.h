#ifndef BGFXTEXTUREHANDLE_H
#define BGFXTEXTUREHANDLE_H

#include "TextureHandle.h"
#include "Texture.h"

namespace gameplay {

class BGFXTextureHandle : public TextureHandle
{
public:
    BGFXTextureHandle(Texture *texture, const unsigned char* data, unsigned int size, Texture::Type type);
    ~BGFXTextureHandle();

    void bind(Uniform * uniform) override;

    const bgfx::TextureHandle getHandle() const { return _handle; }

private:
    bgfx::TextureHandle _handle;

};

} // end namespace gameplay

#endif // BGFXTEXTUREHANDLE_H
