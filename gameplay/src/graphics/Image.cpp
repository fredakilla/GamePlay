#include "../core/Base.h"
#include "../core/FileSystem.h"
#include "../graphics/Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>


namespace gameplay
{
// Callback for reading a png image using Stream
static void readStream(png_structp png, png_bytep data, png_size_t length)
{
    Stream* stream = reinterpret_cast<Stream*>(png_get_io_ptr(png));
    if (stream == NULL || stream->read(data, 1, length) != length)
    {
        png_error(png, "Error reading PNG.");
    }
}

Image* Image::create(const char* path)
{
    GP_ASSERT(path);

    int width, height, channels;

    // resolve file path (path may be an alias)
    const char * filePath = FileSystem::resolvePath(path);

    // load image vertically flipped
   // stbi_set_flip_vertically_on_load(1);

    // load image using stb_image
   // unsigned char* imageData = stbi_load(filePath, &width, &height, &channels, 0);


    unsigned char* imageData = 0;



    // Open the file.
    std::unique_ptr<Stream> stream(FileSystem::open(path));
    if (stream.get() == NULL || !stream->canRead())
    {
        GP_ERROR("Failed to open image file '%s'.", path);
        return NULL;
    }

    // Verify PNG signature.
    unsigned char sig[8];
    if (stream->read(sig, 1, 8) != 8 || png_sig_cmp(sig, 0, 8) == 0)
    {

        // load image vertically flipped
        stbi_set_flip_vertically_on_load(1);

        // load image using stb_image
       imageData = stbi_load(filePath, &width, &height, &channels, 0);







    }
#if 0
    else
    {
       /* size_t fileSize = stream->length() ;
        void *fileData = malloc (sizeof(char)*fileSize);
        stream->read(fileData, 1, fileSize);*/

        int fileSize = 0;
        char * fileData = FileSystem::readAll(path, &fileSize);
        if (fileData == NULL)
        {
            GP_ERROR("Failed to read image from file '%s'.", path);
            return nullptr;
        }

        //bimg::ImageContainer container;
        //bimg::imageParse(container, fileData, stream->length());


        //bx::AllocatorI* alloc = getDefaultAllocator()
        //bimg::ImageContainer* imageContainer = bimg::imageParse(getDefaultAllocator(), fileData, fileSize);

        bimg::ImageContainer* imageContainer = bimg::imageParseDds(getDefaultAllocator(), (void*)fileData, fileSize, 0 );

        /*bimg::ImageContainer imageContainer;
        bimg::imageParse(imageContainer, fileData, fileSize);*/


        imageData = (unsigned char*)imageContainer->m_data;
        channels = 3;


        const bgfx::Memory* mem = bgfx::makeRef(
                              imageContainer->m_data
                            , imageContainer->m_size
                            , imageReleaseCb
                            , imageContainer
                            );
        BX_FREE(getDefaultAllocator(), fileData);

        Format format = Image::RGB;



        // create image from data
        Image* img = Image::create(imageContainer->m_width
                                   , imageContainer->m_height
                                   , format
                                   , mem->data);



        return img;


    }

#endif
















    Format format;
    switch (channels)
    {
    case 3:
        format = Image::RGB;
        break;

    case 4:
        format = Image::RGBA;
        break;

    default:
        GP_ERROR("Unsupported channels count (%d) for image file '%s'.", channels, path);
        return nullptr;
    }


    // flip image vertically
    /*unsigned char flipPixels[width * height * channels];
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            for (int k = 0; k < channels; ++k) {
                flipPixels[(i + j * width) * channels + k] = ht_map[(i + (height - 1 - j) * width) * channels + k];
            }
        }
    }*/

    // create image from data
    Image* img = Image::create(width, height, format, imageData);

    // free stb_image data
    stbi_image_free(imageData);

    return img;


#if 0

    GP_ASSERT(path);

    // Open the file.
    std::unique_ptr<Stream> stream(FileSystem::open(path));
    if (stream.get() == NULL || !stream->canRead())
    {
        GP_ERROR("Failed to open image file '%s'.", path);
        return NULL;
    }

    // Verify PNG signature.
    unsigned char sig[8];
    if (stream->read(sig, 1, 8) != 8 || png_sig_cmp(sig, 0, 8) != 0)
    {
        GP_ERROR("Failed to load file '%s'; not a valid PNG.", path);
        return NULL;
    }

    // Initialize png read struct (last three parameters use stderr+longjump if NULL).
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL)
    {
        GP_ERROR("Failed to create PNG structure for reading PNG file '%s'.", path);
        return NULL;
    }

    // Initialize info struct.
    png_infop info = png_create_info_struct(png);
    if (info == NULL)
    {
        GP_ERROR("Failed to create PNG info structure for PNG file '%s'.", path);
        png_destroy_read_struct(&png, NULL, NULL);
        return NULL;
    }

    // Set up error handling (required without using custom error handlers above).
    if (setjmp(png_jmpbuf(png)))
    {
        GP_ERROR("Failed to set up error handling for reading PNG file '%s'.", path);
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    // Initialize file io.
    png_set_read_fn(png, stream.get(), readStream);

    // Indicate that we already read the first 8 bytes (signature).
    png_set_sig_bytes(png, 8);

    // Read the entire image into memory.
    png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_GRAY_TO_RGB, NULL);

    Image* image = new Image();
    image->_width = png_get_image_width(png, info);
    image->_height = png_get_image_height(png, info);

    png_byte colorType = png_get_color_type(png, info);
    switch (colorType)
    {
    case PNG_COLOR_TYPE_RGBA:
        image->_format = Image::RGBA;
        break;

    case PNG_COLOR_TYPE_RGB:
        image->_format = Image::RGB;
        break;

    default:
        GP_ERROR("Unsupported PNG color type (%d) for image file '%s'.", (int)colorType, path);
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    size_t stride = png_get_rowbytes(png, info);

    // Allocate image data.
    image->_data = new unsigned char[stride * image->_height];

    // Read rows into image data.
    png_bytepp rows = png_get_rows(png, info);
    for (unsigned int i = 0; i < image->_height; ++i)
    {
        memcpy(image->_data+(stride * (image->_height-1-i)), rows[i], stride);
    }

    // Clean up.
    png_destroy_read_struct(&png, &info, NULL);

    return image;

#endif
}

Image* Image::create(unsigned int width, unsigned int height, Image::Format format, unsigned char* data)
{
    GP_ASSERT(width > 0 && height > 0);
    GP_ASSERT(format >= RGB && format <= RGBA);

    unsigned int pixelSize = 0;
    switch(format)
    {
    case Image::RGB:
        pixelSize = 3;
        break;
    case Image::RGBA:
        pixelSize = 4;
        break;
    }

    Image* image = new Image();

    unsigned int dataSize = width * height * pixelSize;

    image->_width = width;
    image->_height = height;
    image->_format = format;
    image->_data = new unsigned char[dataSize];
    if (data)
        memcpy(image->_data, data, dataSize);

    return image;
}

Image::Image() : _data(NULL), _format(RGB), _width(0), _height(0)
{
}

Image::~Image()
{
    SAFE_DELETE_ARRAY(_data);
}

}
