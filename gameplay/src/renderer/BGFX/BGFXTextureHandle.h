#ifndef BGFXTEXTUREHANDLE_H
#define BGFXTEXTUREHANDLE_H

#include "TextureHandle.h"

namespace gameplay {

class BGFXTextureHandle : public TextureHandle
{
public:
    BGFXTextureHandle(Texture *texture);
    ~BGFXTextureHandle();

    void bind(Uniform * uniform) override;

private:
    bgfx::TextureHandle _handle;

};

} // end namespace gameplay

#endif // BGFXTEXTUREHANDLE_H
