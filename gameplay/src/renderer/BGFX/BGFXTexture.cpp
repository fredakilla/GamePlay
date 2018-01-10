#include "BGFXTexture.h"
#include "Texture.h"
#include "BGFXUniform.h"

namespace gameplay {


bgfx::TextureFormat::Enum TEXTURE_BGFX_FORMAT_INFOS[] =
{

    bgfx::TextureFormat::Unknown,   // 0 gameplay::Format::UNKNOWN = 0,
    bgfx::TextureFormat::RGB8   ,   // 1 gameplay::Format::RGB,
    /*bgfx::TextureFormat::RGB8,*/  // 1 gameplay::Format::RGB888 = RGB,
    bgfx::TextureFormat::Unknown,   // 2 gameplay::Format::RGB565,
    bgfx::TextureFormat::RGBA8,     // 3 gameplay::Format::RGBA,
    /*bgfx::TextureFormat::RGBA8,*/ // 3 gameplay::Format::RGBA8888 = RGBA,
    bgfx::TextureFormat::Unknown,   // 4 gameplay::Format::RGBA4444,
    bgfx::TextureFormat::Unknown,   // 5 gameplay::Format::RGBA5551,
    bgfx::TextureFormat::R8,        // 6 gameplay::Format::ALPHA,
    bgfx::TextureFormat::D16,       // 7 gameplay::Format::DEPTH,


    /*bgfx::TextureFormat::Unknown,   // gameplay::Format::UNKNOWN
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


BGFXTexture::BGFXTexture(Texture* texture, const unsigned char *data, unsigned int size, Texture::Type type)
{
    GP_ASSERT(texture);



    bgfx::TextureFormat::Enum bgfxTextureFormat = TEXTURE_BGFX_FORMAT_INFOS[texture->getFormat()];

    GP_ASSERT(bgfxTextureFormat != bgfx::TextureFormat::Unknown);



    if(type == Texture::Type::TEXTURE_2D)
    {

        _handle = bgfx::createTexture2D( texture->getWidth()
                                     , texture->getHeight()
                                     , texture->isMipmapped()
                                     , 1
                                     , bgfxTextureFormat //texture->_format
            , 0//BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP
            //, mem
            );

    }
    else if(type == Texture::Type::TEXTURE_RT)
    {
        _handle = bgfx::createTexture2D(
              texture->getWidth()
            , texture->getHeight()
            , false
            , 1
            , bgfx::TextureFormat::BGRA8
            , BGFX_TEXTURE_RT
            );
    }
    else
    {
        GP_ERROR("Not defined");
    }

    GP_ASSERT(bgfx::isValid(_handle));

    if(data)
    {
        const bgfx::Memory* mem = bgfx::copy(data, size);

        bgfx::updateTexture2D(_handle
                              , 0
                              , 0
                              , 0               // X offset in texture.
                              , 0               // Y offset in texture.
                              , texture->getWidth()      // Width of texture block.
                              , texture->getHeight()     // Height of texture block.
                              , mem             // Texture update data.
                              , UINT16_MAX
                              );
    }
}

BGFXTexture::~BGFXTexture()
{
    if(bgfx::isValid(_handle))
        bgfx::destroy(_handle);
}

void BGFXTexture::bind(Uniform * uniform)
{

    BGFXUniform * bgfxUniform = static_cast<BGFXUniform*>(uniform);

   //bgfx::setTexture(index, dp.material->textures[i]->uniformHandle, dp.material->textures[i]->textureHandle);

    uint32_t flags = UINT32_MAX;

    bgfx::setTexture(uniform->getIndex(), bgfxUniform->getHandle(), _handle, flags);
}


} // end namespace gameplay
