#include "BGFXTextureHandle.h"
#include "Texture.h"
#include "BGFXUniform.h"

namespace gameplay {


bgfx::TextureFormat::Enum TEXTURE_BGFX_FORMAT_INFOS[] =
{
    bgfx::TextureFormat::Unknown,   // gameplay::Format::UNKNOWN
    bgfx::TextureFormat::RGB8,      // gameplay::Format::RGB
    //bgfx::TextureFormat::RGBA8,     // gameplay::Format::RGBA

    /*bgfx::TextureFormat::RGBA8,
    bgfx::TextureFormat::RGBA16,
    bgfx::TextureFormat::RGBA16F,
    bgfx::TextureFormat::RGBA32F,
    bgfx::TextureFormat::R16F,
    bgfx::TextureFormat::RG16F,
    bgfx::TextureFormat::RG32F,
    bgfx::TextureFormat::R8*/
};

/*
enum Format
{
    UNKNOWN = 0,
    RGB,
    RGB888 = RGB,
    RGB565,
    RGBA,
    RGBA8888 = RGBA,
    RGBA4444,
    RGBA5551,
    ALPHA,
    DEPTH,
};*/


BGFXTextureHandle::BGFXTextureHandle(Texture* texture, const unsigned char *data, unsigned int size)
{
    GP_ASSERT(texture);



    bgfx::TextureFormat::Enum bgfxTextureFormat = TEXTURE_BGFX_FORMAT_INFOS[texture->getFormat()];



    const bgfx::Memory* mem = bgfx::copy(data, size);


    _handle = bgfx::createTexture2D( texture->getWidth()
                                     , texture->getHeight()
                                     , texture->isMipmapped()
                                     , 1
                                     , bgfxTextureFormat //texture->_format
            , BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP
            , mem
            );
}

BGFXTextureHandle::~BGFXTextureHandle()
{
    if(bgfx::isValid(_handle))
        bgfx::destroy(_handle);
}

void BGFXTextureHandle::bind(Uniform * uniform)
{

    BGFXUniform * bgfxUniform = static_cast<BGFXUniform*>(uniform);

   //bgfx::setTexture(index, dp.material->textures[i]->uniformHandle, dp.material->textures[i]->textureHandle);

    uint32_t flags = UINT32_MAX;

    bgfx::setTexture(uniform->getIndex(), bgfxUniform->getHandle(), _handle, flags);
}


} // end namespace gameplay
