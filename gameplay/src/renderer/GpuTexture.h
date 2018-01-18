#ifndef GPUTEXTURE_H
#define GPUTEXTURE_H

#include "../core/Base.h"

namespace gameplay
{

class Texture;
class Uniform;

class GpuTexture
{
public:
    //TextureHandle();
    virtual ~GpuTexture() {}

    virtual void bind(Uniform * uniform) = 0;
};

} // end namespace gameplay

#endif // GPUTEXTURE_H
