#include "../../renderer/BGFX/BGFXTexture.h"
#include "../../graphics/Texture.h"
#include "../../renderer/BGFX/BGFXUniform.h"

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




uint32_t MIN_FILTER[] =
{
    BGFX_TEXTURE_MIN_POINT,         //NEAREST = GL_NEAREST,
    BGFX_TEXTURE_MIN_ANISOTROPIC,   //LINEAR = GL_LINEAR,
    BGFX_TEXTURE_NONE,              //NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    BGFX_TEXTURE_NONE,              //LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    BGFX_TEXTURE_NONE,              //NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    BGFX_TEXTURE_MIP_POINT,         //LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

uint32_t MAG_FILTER[] =
{
    BGFX_TEXTURE_MAG_POINT,         //NEAREST = GL_NEAREST,
    BGFX_TEXTURE_MAG_ANISOTROPIC,   //LINEAR = GL_LINEAR,
    BGFX_TEXTURE_NONE,              //NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    BGFX_TEXTURE_NONE,              //LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    BGFX_TEXTURE_NONE,              //NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    BGFX_TEXTURE_MIP_POINT,         //LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

uint32_t WRAP_S[] =
{
    BGFX_TEXTURE_NONE,              //REPEAT = GL_REPEAT,
    BGFX_TEXTURE_U_CLAMP,           //CLAMP = GL_CLAMP_TO_EDGE
    BGFX_TEXTURE_U_MIRROR,
    BGFX_TEXTURE_U_BORDER,
};

uint32_t WRAP_T[] =
{
    BGFX_TEXTURE_NONE,              //REPEAT = GL_REPEAT,
    BGFX_TEXTURE_V_CLAMP,           //CLAMP = GL_CLAMP_TO_EDGE
    BGFX_TEXTURE_V_MIRROR,
    BGFX_TEXTURE_V_BORDER,
};

uint32_t WRAP_R[] =
{
    BGFX_TEXTURE_NONE,              //REPEAT = GL_REPEAT,
    BGFX_TEXTURE_W_CLAMP,           //CLAMP = GL_CLAMP_TO_EDGE
    BGFX_TEXTURE_W_MIRROR,
    BGFX_TEXTURE_W_BORDER,
};
















BGFXTexture::BGFXTexture(Texture* texture, const unsigned char *data, unsigned int size, Texture::Type type)
{
    GP_ASSERT(texture);

    _texture = texture;
    _flags = BGFX_TEXTURE_NONE;


    bgfx::TextureFormat::Enum bgfxTextureFormat = TEXTURE_BGFX_FORMAT_INFOS[texture->getFormat()];

    GP_ASSERT(bgfxTextureFormat != bgfx::TextureFormat::Unknown);



    if(type == Texture::Type::TEXTURE_2D)
    {

        uint32_t flags = BGFX_TEXTURE_NONE;
        flags |= MIN_FILTER[_texture->_minFilter];
        flags |= MAG_FILTER[_texture->_magFilter];
        flags |= WRAP_S[_texture->_wrapS];
        flags |= WRAP_T[_texture->_wrapT];
        flags |= WRAP_R[_texture->_wrapR];
        _flags = flags;

        _handle = bgfx::createTexture2D( texture->getWidth()
                                     , texture->getHeight()
                                     , false//texture->isMipmapped()
                                     , 1
                                     , bgfxTextureFormat //texture->_format
                                     , flags//BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP
                                     , NULL
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

    uint32_t flags = BGFX_TEXTURE_NONE;
    flags |= MIN_FILTER[_texture->_minFilter];
    flags |= MAG_FILTER[_texture->_magFilter];
    flags |= WRAP_S[_texture->_wrapS];
    flags |= WRAP_T[_texture->_wrapT];
    flags |= WRAP_R[_texture->_wrapR];

    bgfx::setTexture(uniform->getIndex(), bgfxUniform->getHandle(), _handle, flags);
}


} // end namespace gameplay
