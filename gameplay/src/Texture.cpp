#include "Base.h"
#include "Image.h"
#include "Texture.h"
#include "FileSystem.h"
#include "BGFX/BGFXTexture.h"



#ifdef GP_USE_MEM_LEAK_DETECTION
#undef new
    #include <bx/allocator.h>
#define new DEBUG_NEW
#else
    #include <bx/allocator.h>
#endif


#include <bimg/decode.h>



namespace gameplay
{

static std::vector<Texture*> __textureCache;
//@@static TextureHandle __currentTextureId = 0;
static Texture::Type __currentTextureType = Texture::TEXTURE_2D;

Texture::Texture() :
    _gpuTtexture(nullptr),
    _format(UNKNOWN),
    _type((Texture::Type)0),
    _width(0),
    _height(0),
    _mipmapped(false),
    _cached(false),
    _compressed(false),
    _wrapS(Texture::REPEAT),
    _wrapT(Texture::REPEAT),
    _wrapR(Texture::REPEAT),
    _minFilter(Texture::NEAREST_MIPMAP_LINEAR),
    _magFilter(Texture::LINEAR)
{
}

Texture::~Texture()
{
    if (_gpuTtexture)
    {
        //@@GL_ASSERT( glDeleteTextures(1, &_handle) );
        delete _gpuTtexture;
        _gpuTtexture = 0;
    }

    // Remove ourself from the texture cache.
    if (_cached)
    {
        std::vector<Texture*>::iterator itr = std::find(__textureCache.begin(), __textureCache.end(), this);
        if (itr != __textureCache.end())
        {
            __textureCache.erase(itr);
        }
    }
}

Texture* Texture::create(const char* path, bool generateMipmaps)
{
    GP_ASSERT( path );

    // Search texture cache first.
    for (size_t i = 0, count = __textureCache.size(); i < count; ++i)
    {
        Texture* t = __textureCache[i];
        GP_ASSERT( t );
        if (t->_path == path)
        {
            // If 'generateMipmaps' is true, call Texture::generateMipamps() to force the
            // texture to generate its mipmap chain if it hasn't already done so.
            if (generateMipmaps)
            {
                t->generateMipmaps();
            }

            // Found a match.
            t->addRef();

            return t;
        }
    }

    // Create texture.
    Texture* texture = createBIMG(path);

    if (texture)
    {
        GP_ASSERT(texture->_gpuTtexture);

        texture->_path = path;
        texture->_cached = true;

        // Add to texture cache.
        __textureCache.push_back(texture);

        return texture;
    }

    GP_ERROR("Failed to load texture from file '%s'.", path);
    return NULL;
}

Texture* Texture::create(Image* image, bool generateMipmaps)
{
    GP_ASSERT( image );

    switch (image->getFormat())
    {
    case Image::RGB:
        return create(Texture::RGB, image->getWidth(), image->getHeight(), image->getData(), generateMipmaps);
    case Image::RGBA:
        return create(Texture::RGBA, image->getWidth(), image->getHeight(), image->getData(), generateMipmaps);
    default:
        GP_ERROR("Unsupported image format (%d).", image->getFormat());
        return NULL;
    }
}

GLint Texture::getFormatInternal(Format format)
{
    switch (format)
    {
        case Texture::RGB888:
        case Texture::RGB565:
            return GL_RGB;
        case Texture::RGBA8888:
        case Texture::RGBA4444:
        case Texture::RGBA5551:
            return GL_RGBA;
        case Texture::ALPHA:
            return GL_RED;
        case Texture::DEPTH:
#if !defined(OPENGL_ES) || defined(GL_ES_VERSION_3_0)
            return GL_DEPTH_COMPONENT32F;
#else
            return GL_DEPTH_COMPONENT;
#endif
        default:
            return 0;
    }
}

GLenum Texture::getFormatTexel(Format format)
{
    switch (format)
    {
        case Texture::RGB888:
        case Texture::RGBA8888:
        case Texture::ALPHA:
            return GL_UNSIGNED_BYTE;
        case Texture::RGB565:
            return GL_UNSIGNED_SHORT_5_6_5;
        case Texture::RGBA4444:
            return GL_UNSIGNED_SHORT_4_4_4_4;
        case Texture::RGBA5551:
            return GL_UNSIGNED_SHORT_5_5_5_1;
        case Texture::DEPTH:
#if !defined(OPENGL_ES) || defined(GL_ES_VERSION_3_0)
            return GL_FLOAT;
#else
            return GL_UNSIGNED_INT;
#endif
        default:
            return 0;
    }
}

size_t Texture::getFormatBPP(Format format)
{
        switch (format)
        {
            case Texture::RGB565:
            case Texture::RGBA4444:
            case Texture::RGBA5551:
                return 2;
            case Texture::RGB888:
                return 3;
            case Texture::RGBA8888:
                return 4;
            case Texture::ALPHA:
                return 1;
            default:
                return 0;
        }
}

Texture* Texture::create(Format format, unsigned int width, unsigned int height, const unsigned char* data, bool generateMipmaps, Texture::Type type)
{
    //@@GP_ASSERT( type == Texture::TEXTURE_2D || type == Texture::TEXTURE_CUBE );

    GLenum target = (GLenum)type;

    GLint internalFormat = getFormatInternal(format);
    GP_ASSERT( internalFormat != 0 );

    GLenum texelType = getFormatTexel(format);
    GP_ASSERT( texelType != 0 );

    //@@
    /*
    // Create the texture.
    GLuint textureId;
    GL_ASSERT( glGenTextures(1, &textureId) );
    GL_ASSERT( glBindTexture(target, textureId) );
    GL_ASSERT( glPixelStorei(GL_UNPACK_ALIGNMENT, 1) );
#ifndef OPENGL_ES
    // glGenerateMipmap is new in OpenGL 3.0. For OpenGL 2.0 we must fallback to use glTexParameteri
    // with GL_GENERATE_MIPMAP prior to actual texture creation (glTexImage2D)
    if ( generateMipmaps && !std::addressof(glGenerateMipmap) )
        GL_ASSERT( glTexParameteri(target, GL_GENERATE_MIPMAP, GL_TRUE) );
#endif
    */
    //@@


    // Load the texture
    size_t bpp = getFormatBPP(format);
    //@@if (type == Texture::TEXTURE_2D)
    //@@{
    //@@    GLenum f = (format == Texture::DEPTH) ? GL_DEPTH_COMPONENT : internalFormat;
    //@@    GL_ASSERT( glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, f, texelType, data) );
    //@@}
    //@@else
    //@@{
    //@@    // Get texture size
    //@@    unsigned int textureSize = width * height;
    //@@    if (bpp == 0)
    //@@    {
    //@@        glDeleteTextures(1, &textureId);
    //@@        GP_ERROR("Failed to determine texture size because format is UNKNOWN.");
    //@@        return NULL;
    //@@    }
    //@@    textureSize *= bpp;
    //@@    // Texture Cube
    //@@    for (unsigned int i = 0; i < 6; i++)
    //@@    {
    //@@        const unsigned char* texturePtr = (data == NULL) ? NULL : &data[i * textureSize];
    //@@        GL_ASSERT( glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, internalFormat, texelType, texturePtr) );
    //@@    }
    //@@}

    // Set initial minification filter based on whether or not mipmaping was enabled.
    Filter minFilter;
    if (format == Texture::DEPTH)
    {
        minFilter = NEAREST;
        /*@@GL_ASSERT( glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST) );
        GL_ASSERT( glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST) );
        GL_ASSERT( glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
        GL_ASSERT( glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
#if !defined(OPENGL_ES) || defined(GL_ES_VERSION_3_0) && GL_ES_VERSION_3_0
        GL_ASSERT( glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_NONE) );
#endif
    @@*/
    }
    else
    {
        minFilter = generateMipmaps ? NEAREST_MIPMAP_LINEAR : LINEAR;
        //@@GL_ASSERT( glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter) );
    }

    Texture* texture = new Texture();
    //@@texture->_handle = textureId;
    texture->_format = format;
    texture->_type = type;
    texture->_width = width;
    texture->_height = height;
    texture->_minFilter = minFilter;
    texture->_internalFormat = internalFormat;
    texture->_texelType = texelType;
    texture->_bpp = bpp;
    if (generateMipmaps)
        texture->generateMipmaps();

    // create bgfx texture

    bimg::TextureFormat::Enum bgfxFormat = (bimg::TextureFormat::Enum)BGFXTexture::toBgfxFormat(format);
    uint8_t numMips = bimg::imageGetNumMips(bgfxFormat, width, height);
    uint32_t imgSize = bimg::imageGetSize(0, width, height, 0, false, false, 1, bgfxFormat);

    bimg::ImageContainer * imageContainer = new bimg::ImageContainer();
    imageContainer->m_size = imgSize; //width * height * bpp;
    imageContainer->m_offset = 0;
    imageContainer->m_width = width;
    imageContainer->m_height = height;
    imageContainer->m_depth = 1;
    imageContainer->m_numLayers = 1;
    imageContainer->m_numMips = 1;
    imageContainer->m_hasAlpha = bpp > 3 ? true : false;
    imageContainer->m_cubeMap = false;
    imageContainer->m_ktx = false;
    imageContainer->m_ktxLE = false;
    imageContainer->m_srgb = false;
    imageContainer->m_format = bgfxFormat;
    imageContainer->m_data = (void*)data;
    imageContainer->m_orientation = bimg::Orientation::R0;

    unsigned int textureSize = width * height * bpp;
    //texture->_gpuTtexture = new BGFXTexture(texture, type, imageContainer);
    texture->_gpuTtexture = new BGFXTexture(texture, data, type);

    delete imageContainer;
    //bimg::imageFree(imageContainer);

    // Restore the texture id
    //@@GL_ASSERT( glBindTexture((GLenum)__currentTextureType, __currentTextureId) );

    return texture;
}

Texture* Texture::create(BGFXTexture *handle, int width, int height, Format format)
{
    GP_ASSERT( handle );

    Texture* texture = new Texture();
    //@@if (glIsTexture(handle))
    //@@{
    //@@    // There is no real way to query for texture type, but an error will be returned if a cube texture is bound to a 2D texture... so check for that
    //@@    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
    //@@    if (glGetError() == GL_NO_ERROR)
    //@@    {
    //@@        texture->_type = TEXTURE_CUBE;
    //@@    }
    //@@    else
    //@@    {
    //@@        // For now, it's either or. But if 3D textures and others are added, it might be useful to simply test a bunch of bindings and seeing which one doesn't error out
    //@@        texture->_type = TEXTURE_2D;
    //@@    }
    //@@
    //@@    // Restore the texture id
    //@@    GL_ASSERT( glBindTexture((GLenum)__currentTextureType, __currentTextureId) );
    //@@}
    texture->_gpuTtexture = handle;
    texture->_format = format;
    texture->_width = width;
    texture->_height = height;
    texture->_internalFormat = getFormatInternal(format);
    texture->_texelType = getFormatTexel(format);
    texture->_bpp = getFormatBPP(format);

    return texture;
}

void Texture::setData(const unsigned char* data)
{
    // Don't work with any compressed or cached textures
    GP_ASSERT( data );
    GP_ASSERT( (!_compressed) );
    GP_ASSERT( (!_cached) );

    GL_ASSERT( glBindTexture((GLenum)_type, _handle) );

    if (_type == Texture::TEXTURE_2D)
    {
        GL_ASSERT( glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, _internalFormat, _texelType, data) );
    }
    else
    {
        // Get texture size
        unsigned int textureSize = _width * _height;
        textureSize *= _bpp;
        // Texture Cube
        for (unsigned int i = 0; i < 6; i++)
        {
            GL_ASSERT( glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, _width, _height, _internalFormat, _texelType, &data[i * textureSize]) );
        }
    }

    if (_mipmapped)
    {
        generateMipmaps();
    }

    // Restore the texture id
    GL_ASSERT( glBindTexture((GLenum)__currentTextureType, __currentTextureId) );
}





bx::AllocatorI* getDefaultAllocator()
{
    static bx::DefaultAllocator s_allocator;
    return &s_allocator;
}

/*
static void imageReleaseCb(void* _ptr, void* _userData)
{
    BX_UNUSED(_ptr);
    bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
    bimg::imageFree(imageContainer);
}*/




Texture* Texture::createBIMG(const char* path)
{
    GP_ASSERT( path );

    // Read file
    int fileSize = 0;
    char * fileData = FileSystem::readAll(path, &fileSize);
    if (fileData == NULL)
    {
        GP_ERROR("Failed to read image from file '%s'.", path);
        return NULL;
    }

    // Parse data
    bimg::ImageContainer* imageContainer = nullptr;
    imageContainer = bimg::imageParse(getDefaultAllocator(), (void*)fileData, fileSize);
    if(imageContainer == nullptr)
    {
        GP_ERROR("Failed to parse image data from file '%s'.", path);
        return NULL;
    }

    Filter minFilter = imageContainer->m_numMips > 1 ? NEAREST_MIPMAP_LINEAR : LINEAR;
    Format format = BGFXTexture::toGp3dFormat(imageContainer->m_format);
    size_t bpp = getFormatBPP(format);
    Type type = TEXTURE_2D;

    // Create gameplay texture.
    Texture* texture = new Texture();
    texture->_format = format;
    texture->_type = type;
    texture->_width = imageContainer->m_width;
    texture->_height = imageContainer->m_height;
    texture->_compressed = false;
    texture->_mipmapped = imageContainer->m_numMips > 1;
    texture->_minFilter = minFilter;
    texture->_bpp = bpp;
    texture->_path = path;

    // create bgfx texture
    unsigned int textureSize = texture->_width * texture->_height * bpp;
    texture->_gpuTtexture = new BGFXTexture(path, texture);//, type, imageContainer);

    // free file data
    free(fileData);
    bimg::imageFree(imageContainer);

    return texture;
}





Texture::Format Texture::getFormat() const
{
    return _format;
}

Texture::Type Texture::getType() const
{
    return _type;
}

const char* Texture::getPath() const
{
    return _path.c_str();
}

unsigned int Texture::getWidth() const
{
    return _width;
}

unsigned int Texture::getHeight() const
{
    return _height;
}

BGFXTexture *Texture::getHandle() const
{
    return _gpuTtexture;
}

void Texture::generateMipmaps()
{
    if (!_mipmapped)
    {
        _mipmapped = true;
    }

    //@@if (!_mipmapped)
    //@@{
    //@@    GLenum target = (GLenum)_type;
    //@@    GL_ASSERT( glBindTexture(target, _handle) );
    //@@    GL_ASSERT( glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST) );
    //@@    if( std::addressof(glGenerateMipmap) )
    //@@        GL_ASSERT( glGenerateMipmap(target) );
    //@@
    //@@    _mipmapped = true;
    //@@
    //@@    // Restore the texture id
    //@@    GL_ASSERT( glBindTexture((GLenum)__currentTextureType, __currentTextureId) );
    //@@}
}

bool Texture::isMipmapped() const
{
    return _mipmapped;
}

bool Texture::isCompressed() const
{
    return _compressed;
}




//-------------------------------------------------------------------------
//
// Texture Sampler
//
//-------------------------------------------------------------------------

Texture::Sampler::Sampler(Texture* texture)
    : _texture(texture), _wrapS(Texture::REPEAT), _wrapT(Texture::REPEAT), _wrapR(Texture::REPEAT)
{
    GP_ASSERT( texture );
    _minFilter = texture->_minFilter;
    _magFilter = texture->_magFilter;
}

Texture::Sampler::~Sampler()
{
    SAFE_RELEASE(_texture);
}

Texture::Sampler* Texture::Sampler::create(Texture* texture)
{
    GP_ASSERT( texture );
    GP_ASSERT( texture->_type == Texture::TEXTURE_2D || texture->_type == Texture::TEXTURE_CUBE || texture->_type == Texture::TEXTURE_RT);
    texture->addRef();
    return new Sampler(texture);
}

Texture::Sampler* Texture::Sampler::create(const char* path, bool generateMipmaps)
{
    Texture* texture = Texture::create(path, generateMipmaps);
    return texture ? new Sampler(texture) : NULL;
}

void Texture::Sampler::setWrapMode(Wrap wrapS, Wrap wrapT, Wrap wrapR)
{
    _wrapS = wrapS;
    _wrapT = wrapT;
    _wrapR = wrapR;
}

void Texture::Sampler::setFilterMode(Filter minificationFilter, Filter magnificationFilter)
{
    _minFilter = minificationFilter;
    _magFilter = magnificationFilter;
}

Texture* Texture::Sampler::getTexture() const
{
    return _texture;
}

void Texture::Sampler::bind(Uniform * uniform)
{
    GP_ASSERT( _texture );


    if (_texture->_minFilter != _minFilter)
    {
        _texture->_minFilter = _minFilter;
    }
    if (_texture->_magFilter != _magFilter)
    {
        _texture->_magFilter = _magFilter;
    }
    if (_texture->_wrapS != _wrapS)
    {
        _texture->_wrapS = _wrapS;
    }
    if (_texture->_wrapT != _wrapT)
    {
        _texture->_wrapT = _wrapT;
    }
    if (_texture->_wrapR != _wrapR)
    {
        _texture->_wrapR = _wrapR;
    }

    _texture->_gpuTtexture->bind(uniform);


    //@@
#if 0
    GLenum target = (GLenum)_texture->_type;
    if (__currentTextureId != _texture->_handle)
    {
        GL_ASSERT( glBindTexture(target, _texture->_handle) );
        __currentTextureId = _texture->_handle;
        __currentTextureType = _texture->_type;
    }

    if (_texture->_minFilter != _minFilter)
    {
        _texture->_minFilter = _minFilter;
        GL_ASSERT( glTexParameteri(target, GL_TEXTURE_MIN_FILTER, (GLenum)_minFilter) );
    }

    if (_texture->_magFilter != _magFilter)
    {
        _texture->_magFilter = _magFilter;
        GL_ASSERT( glTexParameteri(target, GL_TEXTURE_MAG_FILTER, (GLenum)_magFilter) );
    }

    if (_texture->_wrapS != _wrapS)
    {
        _texture->_wrapS = _wrapS;
        GL_ASSERT( glTexParameteri(target, GL_TEXTURE_WRAP_S, (GLenum)_wrapS) );
    }

    if (_texture->_wrapT != _wrapT)
    {
        _texture->_wrapT = _wrapT;
        GL_ASSERT( glTexParameteri(target, GL_TEXTURE_WRAP_T, (GLenum)_wrapT) );
    }

#if defined(GL_TEXTURE_WRAP_R) // OpenGL ES 3.x and up, OpenGL 1.2 and up
    if (_texture->_wrapR != _wrapR)
    {
        _texture->_wrapR = _wrapR;
        if (target == GL_TEXTURE_CUBE_MAP) // We don't want to run this on something that we know will fail
            GL_ASSERT( glTexParameteri(target, GL_TEXTURE_WRAP_R, (GLenum)_wrapR) );
    }
#endif
#endif
    //@@
}

}
