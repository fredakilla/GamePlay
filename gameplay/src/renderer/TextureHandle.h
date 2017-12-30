#ifndef TEXTUREHANDLE_H
#define TEXTUREHANDLE_H

#include "Base.h"

namespace gameplay
{

class Texture;
class Uniform;

class TextureHandle
{
public:
    //TextureHandle();
    virtual ~TextureHandle() {}

    virtual void bind(Uniform * uniform) = 0;
};

} // end namespace gameplay

#endif // TEXTUREHANDLE_H
