#ifndef BGFXTEXTUREHANDLE_H
#define BGFXTEXTUREHANDLE_H

#include "../Base.h"
#include "../Texture.h"
#include "bimg/decode.h"

namespace gameplay
{

class BGFXTexture
{
public:

    BGFXTexture(const char * path, Texture *texture);
    BGFXTexture(Texture* texture, const unsigned char *data, Texture::Type type);

    //---
    static Texture* createFromFile(const char * path);
    static Texture* createFromData(const unsigned char* data, Texture::GPTextureInfos infos);



    BGFXTexture(Texture *texture, Texture::Type type, bimg::ImageContainer * imageContainer);
    ~BGFXTexture();

    void bind(Uniform * uniform);

    const bgfx::TextureHandle getHandle() const { return _handle; }

    static bgfx::TextureFormat::Enum toBgfxFormat(Texture::Format gp3dFormat);
    static Texture::Format toGp3dFormat(bgfx::TextureFormat::Enum bimgTextureFormat);

private:


    BGFXTexture();


    bgfx::TextureHandle _handle;
    Texture* _texture;

    uint32_t _flags;

};

} // end namespace gameplay

#endif // BGFXTEXTUREHANDLE_H
