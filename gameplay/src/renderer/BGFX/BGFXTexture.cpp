#include "../../renderer/BGFX/BGFXTexture.h"
#include "../../graphics/Texture.h"
#include "../../renderer/BGFX/BGFXUniform.h"

//#include <bx/allocator.h>
//#include <bimg/decode.h>
//#include <bgfx/platform.h>

namespace gameplay {


/*  GP3D <=> BGFX - texture format
----------------------------------------------------------------------
    Texture::Format::UNKNOWN   == bgfx::TextureFormat::Unknown
    Texture::Format::RGB       == bgfx::TextureFormat::RGB8
    Texture::Format::RGB888    == bgfx::TextureFormat::RGB8
    Texture::Format::RGB565    == bgfx::TextureFormat::R5G6B5
    Texture::Format::RGBA      == bgfx::TextureFormat::RGBA8
    Texture::Format::RGBA8888  == bgfx::TextureFormat::RGBA8
    Texture::Format::RGBA4444  == bgfx::TextureFormat::RGBA4
    Texture::Format::RGBA5551  == bgfx::TextureFormat::RGB5A1
    Texture::Format::ALPHA     == bgfx::TextureFormat::R8
    Texture::Format::DEPTH     == bgfx::TextureFormat::D16
----------------------------------------------------------------------
*/

bgfx::TextureFormat::Enum BGFXTexture::toBgfxFormat(Texture::Format gp3dFormat)
{
    switch(gp3dFormat)
    {
    case Texture::Format::UNKNOWN    : return bgfx::TextureFormat::Unknown;
    case Texture::Format::RGB        : return bgfx::TextureFormat::RGB8;
    case Texture::Format::RGB565     : return bgfx::TextureFormat::R5G6B5;
    case Texture::Format::RGBA       : return bgfx::TextureFormat::RGBA8;
    case Texture::Format::RGBA4444   : return bgfx::TextureFormat::RGBA4;
    case Texture::Format::RGBA5551   : return bgfx::TextureFormat::RGB5A1;
    case Texture::Format::ALPHA      : return bgfx::TextureFormat::R8;
    case Texture::Format::DEPTH      : return bgfx::TextureFormat::D16;
    default:
        GP_ASSERT(!"gp3d texture format unknown.");
        return bgfx::TextureFormat::Unknown;
    }
}

Texture::Format BGFXTexture::toGp3dFormat(bimg::TextureFormat::Enum bimgTextureFormat)
{
    switch(bimgTextureFormat)
    {
    case bgfx::TextureFormat::Unknown   : return Texture::Format::UNKNOWN;
    case bgfx::TextureFormat::RGB8      : return Texture::Format::RGB;
    case bgfx::TextureFormat::R5G6B5    : return Texture::Format::RGB565;
    case bgfx::TextureFormat::RGBA8     : return Texture::Format::RGBA;
    case bgfx::TextureFormat::RGBA4     : return Texture::Format::RGBA4444;
    case bgfx::TextureFormat::RGB5A1    : return Texture::Format::RGBA5551;
    case bgfx::TextureFormat::R8        : return Texture::Format::ALPHA;
    case bgfx::TextureFormat::D16       : return Texture::Format::DEPTH;

    case bgfx::TextureFormat::BC2       : return Texture::Format::RGBA;

    default:
        GP_ASSERT(!"bimg texture format not supported.");
        return Texture::Format::UNKNOWN;
    }
}


uint32_t MIN_FILTER[] =
{
    BGFX_TEXTURE_MIN_POINT,         //NEAREST = GL_NEAREST,
    BGFX_TEXTURE_MIN_ANISOTROPIC,   //LINEAR = GL_LINEAR,
    BGFX_TEXTURE_MIP_POINT,         //NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    BGFX_TEXTURE_MIP_POINT,         //LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    BGFX_TEXTURE_NONE,              //NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    BGFX_TEXTURE_NONE,              //LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

uint32_t MAG_FILTER[] =
{
    BGFX_TEXTURE_MAG_POINT,         //NEAREST = GL_NEAREST,
    BGFX_TEXTURE_MAG_ANISOTROPIC,   //LINEAR = GL_LINEAR,
    BGFX_TEXTURE_MIP_POINT,         //NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    BGFX_TEXTURE_MIP_POINT,         //LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    BGFX_TEXTURE_NONE,              //NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    BGFX_TEXTURE_NONE,              //LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
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










/*static void imageReleaseCb(void* _ptr, void* _userData)
{
    BX_UNUSED(_ptr);
    bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
    bimg::imageFree(imageContainer);
}*/





BGFXTexture::BGFXTexture(Texture* texture, Texture::Type type, bimg::ImageContainer *imageContainer)
{
    GP_ASSERT(texture);

    _texture = texture;
    _flags = BGFX_TEXTURE_NONE;


    bgfx::TextureFormat::Enum bgfxTextureFormat = toBgfxFormat(texture->getFormat());
    GP_ASSERT(bgfxTextureFormat != bgfx::TextureFormat::Unknown);



    if(type == Texture::Type::TEXTURE_2D)
    {
        uint32_t flags = BGFX_TEXTURE_NONE
                | MIN_FILTER[_texture->_minFilter]
                | MAG_FILTER[_texture->_magFilter]
                | WRAP_S[_texture->_wrapS]
                | WRAP_T[_texture->_wrapT]
                | WRAP_R[_texture->_wrapR];
        _flags = flags;

        bgfx::TextureInfo info;
        bgfx::calcTextureSize(
                    info
                    , uint16_t(imageContainer->m_width)
                    , uint16_t(imageContainer->m_height)
                    , uint16_t(imageContainer->m_depth)
                    , imageContainer->m_cubeMap
                    , 1 < imageContainer->m_numMips
                    , imageContainer->m_numLayers
                    , bgfx::TextureFormat::Enum(imageContainer->m_format));
        GP_ASSERT(info.storageSize == imageContainer->m_size);


       // const bgfx::Memory* mem = bgfx::makeRef(
       //                           imageContainer->m_data
       //                         , imageContainer->m_size
       //                         , 0//imageReleaseCb
       //                         , imageContainer
       //     );



        //const bgfx::Memory * mem = bgfx::alloc(info.storageSize);
        //memcpy(mem->data, imageContainer->m_data, imageContainer->m_size);




       const bgfx::Memory* mem = bgfx::copy(imageContainer->m_data, imageContainer->m_size);


        _handle = bgfx::createTexture2D(
                    uint16_t(imageContainer->m_width)
                    , uint16_t(imageContainer->m_height)
                    , 1 < imageContainer->m_numMips
                    , imageContainer->m_numLayers
                    , bgfx::TextureFormat::Enum(imageContainer->m_format)
                    , _flags
                    , mem
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

#if 0
    if(data)
    {
       /* bgfx::TextureInfo info;
        uint16_t depth = 0;
        bool cubeMap = false;
        bool hasMips = texture->isMipmapped();
        uint16_t numLayers = 1;
        bgfx::TextureFormat::Enum format = TEXTURE_BGFX_FORMAT_INFOS[texture->getFormat()];
        bgfx::calcTextureSize(info, texture->getWidth(), texture->getHeight(), depth, cubeMap, hasMips, numLayers, format);

*/


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
#endif
}

BGFXTexture::~BGFXTexture()
{
    if(bgfx::isValid(_handle))
        bgfx::destroy(_handle);
}

void BGFXTexture::bind(Uniform * uniform)
{
    BGFXUniform * bgfxUniform = static_cast<BGFXUniform*>(uniform);

    uint32_t flags = BGFX_TEXTURE_NONE
            | MIN_FILTER[_texture->_minFilter]
            | MAG_FILTER[_texture->_magFilter]
            | WRAP_S[_texture->_wrapS]
            | WRAP_T[_texture->_wrapT]
            | WRAP_R[_texture->_wrapR];

    bgfx::setTexture(uniform->getIndex(), bgfxUniform->getHandle(), _handle, flags);
}

} // end namespace gameplay
