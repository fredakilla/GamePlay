#include "../BGFX/BGFXTexture.h"
#include "../Texture.h"
#include "../BGFX/BGFXUniform.h"

//#include <bx/allocator.h>
//#include <bimg/decode.h>
//#include <bgfx/platform.h>


#include "FileSystem.h"
#ifdef GP_USE_MEM_LEAK_DETECTION
#undef new
    #include <bx/allocator.h>
#define new DEBUG_NEW
#else
    #include <bx/allocator.h>
#endif
#include <bimg/decode.h>

bx::AllocatorI* getDefaultAllocator()
{
    static bx::DefaultAllocator s_allocator;
    return &s_allocator;
}

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

Texture::Format BGFXTexture::toGp3dFormat(bgfx::TextureFormat::Enum bimgTextureFormat)
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
    case bgfx::TextureFormat::BC3       : return Texture::Format::RGBA;
    case bgfx::TextureFormat::BC5       : return Texture::Format::RGBA;

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




bgfx::TextureHandle createTexture(bimg::ImageContainer* imageContainer, uint32_t flags, bgfx::TextureInfo* info = NULL)
{
    bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

    const bgfx::Memory* mem = bgfx::makeRef(
                imageContainer->m_data
                , imageContainer->m_size
                , 0
                , imageContainer
                );

    if (imageContainer->m_cubeMap)
    {
        handle = bgfx::createTextureCube(
                    uint16_t(imageContainer->m_width)
                    , 1 < imageContainer->m_numMips
                    , imageContainer->m_numLayers
                    , bgfx::TextureFormat::Enum(imageContainer->m_format)
                    , flags
                    , mem
                    );
    }
    else if (1 < imageContainer->m_depth)
    {
        handle = bgfx::createTexture3D(
                    uint16_t(imageContainer->m_width)
                    , uint16_t(imageContainer->m_height)
                    , uint16_t(imageContainer->m_depth)
                    , 1 < imageContainer->m_numMips
                    , bgfx::TextureFormat::Enum(imageContainer->m_format)
                    , flags
                    , mem
                    );
    }
    else
    {
        handle = bgfx::createTexture2D(
                    uint16_t(imageContainer->m_width)
                    , uint16_t(imageContainer->m_height)
                    , 1 < imageContainer->m_numMips
                    , imageContainer->m_numLayers
                    , bgfx::TextureFormat::Enum(imageContainer->m_format)
                    , flags
                    , mem
                    );
    }


    if (NULL != info)
    {
        bgfx::calcTextureSize(
                    *info
                    , uint16_t(imageContainer->m_width)
                    , uint16_t(imageContainer->m_height)
                    , uint16_t(imageContainer->m_depth)
                    , imageContainer->m_cubeMap
                    , 1 < imageContainer->m_numMips
                    , imageContainer->m_numLayers
                    , bgfx::TextureFormat::Enum(imageContainer->m_format)
                    );
    }


    GP_ASSERT(bgfx::isValid(handle));
    return handle;
}





bgfx::TextureHandle loadTexture(const char* filePath, uint32_t flags, bgfx::TextureInfo* info = NULL, bimg::Orientation::Enum* orientation = NULL)
{
    bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

    // Read file
    int size = 0;
    char * data = FileSystem::readAll(filePath, &size);
    if (data == NULL)
    {
        GP_ERROR("Failed to read image from file '%s'.", filePath);
        return handle;
    }

    // Parse data
    bimg::ImageContainer* imageContainer = bimg::imageParse(getDefaultAllocator(), data, size);
    if(imageContainer == nullptr)
    {
        GP_ERROR("Failed to parse image data from file '%s'.", filePath);
        return handle;
    }

    // create bgfx texture
    handle = createTexture(imageContainer, flags, info);


    free(data);
    //bimg::imageFree(imageContainer);



    return handle;

/*

    // create bgfx texture
    if (NULL != imageContainer)
    {
        if (NULL != orientation)
        {
            *orientation = imageContainer->m_orientation;
        }

        const bgfx::Memory* mem = bgfx::makeRef(
                    imageContainer->m_data
                    , imageContainer->m_size
                    , 0
                    , imageContainer
                    );
        free(data);

        if (imageContainer->m_cubeMap)
        {
            handle = bgfx::createTextureCube(
                        uint16_t(imageContainer->m_width)
                        , 1 < imageContainer->m_numMips
                        , imageContainer->m_numLayers
                        , bgfx::TextureFormat::Enum(imageContainer->m_format)
                        , flags
                        , mem
                        );
        }
        else if (1 < imageContainer->m_depth)
        {
            handle = bgfx::createTexture3D(
                        uint16_t(imageContainer->m_width)
                        , uint16_t(imageContainer->m_height)
                        , uint16_t(imageContainer->m_depth)
                        , 1 < imageContainer->m_numMips
                        , bgfx::TextureFormat::Enum(imageContainer->m_format)
                        , flags
                        , mem
                        );
        }
        else
        {
            handle = bgfx::createTexture2D(
                        uint16_t(imageContainer->m_width)
                        , uint16_t(imageContainer->m_height)
                        , 1 < imageContainer->m_numMips
                        , imageContainer->m_numLayers
                        , bgfx::TextureFormat::Enum(imageContainer->m_format)
                        , flags
                        , mem
                        );
        }

        bgfx::setName(handle, filePath);

        if (NULL != info)
        {
            bgfx::calcTextureSize(
                        *info
                        , uint16_t(imageContainer->m_width)
                        , uint16_t(imageContainer->m_height)
                        , uint16_t(imageContainer->m_depth)
                        , imageContainer->m_cubeMap
                        , 1 < imageContainer->m_numMips
                        , imageContainer->m_numLayers
                        , bgfx::TextureFormat::Enum(imageContainer->m_format)
                        );
        }
    }

    return handle;

    */
}







BGFXTexture::BGFXTexture(const char * path, Texture* texture)
{
    GP_ASSERT(path);
    GP_ASSERT(texture);

    _texture = texture;

    uint32_t flags = BGFX_TEXTURE_NONE
            | MIN_FILTER[_texture->_minFilter]
            | MAG_FILTER[_texture->_magFilter]
            | WRAP_S[_texture->_wrapS]
            | WRAP_T[_texture->_wrapT]
            | WRAP_R[_texture->_wrapR];
    _flags = flags;

    _handle = loadTexture(path, flags);
}





BGFXTexture::BGFXTexture(Texture* texture, const unsigned char* data, Texture::Type type)
{
    GP_ASSERT(texture);
    GP_ASSERT(data);

    _texture = texture;

    uint32_t flags = BGFX_TEXTURE_NONE
            | MIN_FILTER[_texture->_minFilter]
            | MAG_FILTER[_texture->_magFilter]
            | WRAP_S[_texture->_wrapS]
            | WRAP_T[_texture->_wrapT]
            | WRAP_R[_texture->_wrapR];
    _flags = flags;





    unsigned int width = texture->_width;
    unsigned int height = texture->_height;


    uint32_t imgSize = width * height * texture->_bpp;
    // or
    //uint32_t textureSize = bimg::imageGetSize(0, width, height, 0, false, false, 1, bgfxFormat);


    bimg::TextureFormat::Enum bgfxFormat = (bimg::TextureFormat::Enum)toBgfxFormat(texture->_format);
    uint8_t numMips = bimg::imageGetNumMips(bgfxFormat, width, height);
    //uint32_t imgSize = bimg::imageGetSize(0, width, height, 1, false, false, 1, bgfxFormat);


    bimg::ImageContainer * imageContainer = new bimg::ImageContainer();
    imageContainer->m_size = imgSize; //width * height * bpp;
    imageContainer->m_offset = 0;
    imageContainer->m_width = width;
    imageContainer->m_height = height;
    imageContainer->m_depth = 1;
    imageContainer->m_numLayers = 1;
    imageContainer->m_numMips = 1;
    imageContainer->m_hasAlpha = texture->_bpp > 3 ? true : false;
    imageContainer->m_cubeMap = false;
    imageContainer->m_ktx = false;
    imageContainer->m_ktxLE = false;
    imageContainer->m_srgb = false;
    imageContainer->m_format = bgfxFormat;
    //imageContainer->m_data = (void*)data;
    imageContainer->m_orientation = bimg::Orientation::R0;

    //const bgfx::Memory* mem = bgfx::copy(imageContainer->m_data, imageContainer->m_size);


    const bgfx::Memory* mem = bgfx::copy(data, imgSize);
    imageContainer->m_data = mem->data;

    //memcpy(imageContainer->m_data, data, imgSize);




    /*bgfx::TextureInfo info;
    bgfx::calcTextureSize(
                info
                , uint16_t(imageContainer->m_width)
                , uint16_t(imageContainer->m_height)
                , uint16_t(imageContainer->m_depth)
                , imageContainer->m_cubeMap
                , 1 < imageContainer->m_numMips
                , imageContainer->m_numLayers
                , bgfx::TextureFormat::Enum(imageContainer->m_format));
    GP_ASSERT(info.storageSize == imageContainer->m_size);*/






    _handle = createTexture(imageContainer, flags);




}




// ----------------


BGFXTexture::BGFXTexture() :
    _handle(BGFX_INVALID_HANDLE)
{
}


Texture * BGFXTexture::createFromFile(const char * path)
{
    BGFXTexture * bgfxTexture = new BGFXTexture();

    // create texture from file
    bgfx::TextureInfo info;
    bgfxTexture->_handle = loadTexture(path, BGFX_TEXTURE_NONE, &info);

    // create gameplay3d texture
    Texture* texture = new Texture();
    texture->_format = BGFXTexture::toGp3dFormat(info.format);
    texture->_type = Texture::TEXTURE_2D;
    texture->_width = info.width;
    texture->_height = info.height;
    texture->_compressed = false;
    texture->_mipmapped = info.numMips > 1;
    texture->_bpp = info.bitsPerPixel / 8;
    texture->_path = path;
    texture->_gpuTtexture = bgfxTexture;

    bgfxTexture->_texture = texture;

    return texture;
}


Texture* BGFXTexture::createFromData(const unsigned char* data, Texture::GPTextureInfos infos)
{
    BGFXTexture * bgfxTexture = new BGFXTexture();


    unsigned int width = infos.width;
    unsigned int height = infos.height;
    //uint32_t imgSize = width * height * infos.bytePerPixel;
    bimg::TextureFormat::Enum bgfxFormat = (bimg::TextureFormat::Enum)toBgfxFormat(infos.format);



    uint32_t imgSize = bimg::imageGetSize(0, width, height, 1, false, false, 1, bgfxFormat);



    bimg::ImageContainer * imageContainer = new bimg::ImageContainer();
    imageContainer->m_size = imgSize;
    imageContainer->m_offset = 0;
    imageContainer->m_width = width;
    imageContainer->m_height = height;
    imageContainer->m_depth = 1;
    imageContainer->m_numLayers = 1;
    imageContainer->m_numMips = 1;
    imageContainer->m_hasAlpha = infos.bytePerPixel > 3 ? true : false;
    imageContainer->m_cubeMap = false;
    imageContainer->m_ktx = false;
    imageContainer->m_ktxLE = false;
    imageContainer->m_srgb = false;
    imageContainer->m_format = bgfxFormat;
    //imageContainer->m_data = (void*)data;
    imageContainer->m_orientation = bimg::Orientation::R0;

    const bgfx::Memory* mem = bgfx::copy(data, imgSize);
    imageContainer->m_data = mem->data;

    bgfx::TextureInfo bgfxInfo;
    bgfxTexture->_handle = createTexture(imageContainer, BGFX_TEXTURE_NONE, &bgfxInfo);


    // create gameplay3d texture
    Texture* texture = new Texture();
    texture->_format = infos.format; //BGFXTexture::toGp3dFormat(bgfxInfo.format);
    texture->_type = Texture::TEXTURE_2D;
    texture->_width = bgfxInfo.width;
    texture->_height = bgfxInfo.height;
    texture->_compressed = false;
    texture->_mipmapped = bgfxInfo.numMips > 1;
    texture->_bpp = bgfxInfo.bitsPerPixel / 8;
    texture->_gpuTtexture = bgfxTexture;

    bgfxTexture->_texture = texture;

    return texture;
}














#if 1
BGFXTexture::BGFXTexture(Texture* texture, Texture::Type type, bimg::ImageContainer *imageContainer)
{
    GP_ASSERT(texture);

    _texture = texture;
    _flags = BGFX_TEXTURE_NONE;


    if( strcmp(texture->getPath(), "") )
    {
        bimg::Orientation::Enum orientation;
        _handle = loadTexture(texture->getPath(), BGFX_TEXTURE_NONE, NULL, &orientation);
        return;
    }



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
}


#endif






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
