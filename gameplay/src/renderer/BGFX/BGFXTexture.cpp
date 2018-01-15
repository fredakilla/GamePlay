#include "../../renderer/BGFX/BGFXTexture.h"
#include "../../graphics/Texture.h"
#include "../../renderer/BGFX/BGFXUniform.h"

#include <bx/allocator.h>
#include <bimg/decode.h>
#include <bgfx/platform.h>

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

};


bgfx::TextureFormat::Enum BGFXTexture::toBgfxFormat(Texture::Format gp3dFormat)
{
    return TEXTURE_BGFX_FORMAT_INFOS[gp3dFormat];
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










static void imageReleaseCb(void* _ptr, void* _userData)
{
    BX_UNUSED(_ptr);
    bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
    bimg::imageFree(imageContainer);
}





BGFXTexture::BGFXTexture(Texture* texture, const unsigned char *data, unsigned int size, Texture::Type type, bimg::ImageContainer *imageContainer)
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


        if(imageContainer == 0)
        {





        _handle = bgfx::createTexture2D( texture->getWidth()
                                             , texture->getHeight()
                                             , texture->isMipmapped()
                                             , 1
                                             , bgfxTextureFormat //texture->_format
                                             , _flags //_flags //BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP
                                             , 0
                                             );

        }
        else
        {
            bgfx::TextureInfo info;
            bgfx::calcTextureSize(
                                  info
                                , uint16_t(imageContainer->m_width)
                                , uint16_t(imageContainer->m_height)
                                , uint16_t(imageContainer->m_depth)
                                , imageContainer->m_cubeMap
                                , 1 < imageContainer->m_numMips
                                , imageContainer->m_numLayers
                                , bgfx::TextureFormat::Enum(imageContainer->m_format)
                    );




            /*const bgfx::Memory* mem = bgfx::makeRef(
                                  imageContainer->m_data
                                , imageContainer->m_size
                                , imageReleaseCb
                                , imageContainer
            );*/



            const bgfx::Memory * mem = bgfx::alloc(info.storageSize);
            memcpy(mem->data, imageContainer->m_data, imageContainer->m_size);




            /*const bgfx::Memory* mem = bgfx::copy(
                                              imageContainer->m_data
                                            , info.storageSize /// imageContainer->m_size
                        );*/


            _handle = bgfx::createTexture2D(
                                  uint16_t(imageContainer->m_width)
                                , uint16_t(imageContainer->m_height)
                                , 1 < imageContainer->m_numMips
                                , imageContainer->m_numLayers
                                , bgfx::TextureFormat::Enum(imageContainer->m_format)
                                , _flags
                                , mem
            );


            if(imageContainer->m_ktx)
            _handle = bgfx::createTexture(mem, _flags);



                    return;
        }

#if 0

        bgfx::TextureInfo info;
        /*uint16_t depth = 1;
        bool cubeMap = false;
        bool hasMips = texture->isMipmapped();
        uint16_t numLayers = 1;
        bgfx::TextureFormat::Enum format = TEXTURE_BGFX_FORMAT_INFOS[texture->getFormat()];
        bgfx::calcTextureSize(info, texture->getWidth(), texture->getHeight(), depth, cubeMap, hasMips, numLayers, format);*/

        bgfx::calcTextureSize(
                              info
                            , uint16_t(imageContainer->m_width)
                            , uint16_t(imageContainer->m_height)
                            , uint16_t(imageContainer->m_depth)
                            , imageContainer->m_cubeMap
                            , 1 < imageContainer->m_numMips
                            , imageContainer->m_numLayers
                            , bgfx::TextureFormat::Enum(imageContainer->m_format)
        );




        //const bgfx::Memory* mem = bgfx::copy(data, info.storageSize);

        /*const bgfx::Memory* mem = bgfx::makeRef(
                              imageContainer->m_data
                            , imageContainer->m_size// info.storageSize
                            , 0
                            , 0
        );*/

        const bgfx::Memory* mem = bgfx::makeRef(
                              imageContainer->m_data
                            , imageContainer->m_size
                            , imageReleaseCb
                            , imageContainer
        );



        /*_handle = bgfx::createTexture2D( texture->getWidth()
                                     , texture->getHeight()
                                     , texture->isMipmapped()
                                     , 1
                                     , bgfxTextureFormat //texture->_format
                                     , BGFX_TEXTURE_NONE //_flags //BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP
                                     , mem
                                     );*/


        _handle = bgfx::createTexture2D(
                              uint16_t(imageContainer->m_width)
                            , uint16_t(imageContainer->m_height)
                            , 1 < imageContainer->m_numMips
                            , imageContainer->m_numLayers
                            , bgfx::TextureFormat::Enum(imageContainer->m_format)
                            , _flags
                            , mem
        );

#endif



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

    uint32_t flags = BGFX_TEXTURE_NONE;
    flags |= MIN_FILTER[_texture->_minFilter];
    flags |= MAG_FILTER[_texture->_magFilter];
    flags |= WRAP_S[_texture->_wrapS];
    flags |= WRAP_T[_texture->_wrapT];
    flags |= WRAP_R[_texture->_wrapR];

    bgfx::setTexture(uniform->getIndex(), bgfxUniform->getHandle(), _handle, flags);
}


} // end namespace gameplay
