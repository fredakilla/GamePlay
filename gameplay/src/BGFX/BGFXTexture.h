#ifndef BGFXTEXTUREHANDLE_H
#define BGFXTEXTUREHANDLE_H

#include "../Base.h"
#include "../Texture.h"

namespace gameplay
{

class BGFXTexture
{
public:
    ~BGFXTexture();

    static Texture* createFromFile(const char* path);
    static Texture* createFromData(const unsigned char* data, Texture::GPTextureInfo info);

    void bind(Uniform* uniform, Texture* texture);
    const bgfx::TextureHandle getHandle() const { return _handle; }
    static bgfx::TextureFormat::Enum toBgfxFormat(Texture::Format gp3dFormat);
    static Texture::Format toGp3dFormat(bgfx::TextureFormat::Enum bimgTextureFormat);

private:

    BGFXTexture();

    bgfx::TextureHandle _handle;
};

} // end namespace gameplay

#endif // BGFXTEXTUREHANDLE_H
