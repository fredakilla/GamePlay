#include "../core/Base.h"
#include "../core/FileSystem.h"
#include "../graphics/Image.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image/stb_image.h>

namespace gameplay {

Image* Image::create(const char* path)
{

#if 0
    GP_ASSERT(path);

    int width, height, channels;
    unsigned char* imageData = 0;

    // resolve file path (path may be an alias)
    const char * filePath = FileSystem::resolvePath(path);

    // load image vertically flipped
    stbi_set_flip_vertically_on_load(1);

    // load image using stb_image
    imageData = stbi_load(filePath, &width, &height, &channels, 0);
    GP_ASSERT(imageData);

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

    // manually flip image vertically
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
