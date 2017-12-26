#ifndef BGFX_INDEXBUFFER_H_
#define BGFX_INDEXBUFFER_H_

#include "IndexBuffer.h"

namespace gameplay {

class BGFXIndexBuffer : public IndexBuffer
{
public:
    BGFXIndexBuffer(const unsigned int indexFormat, unsigned int indexCount, bool dynamic);
    ~BGFXIndexBuffer();
    virtual GLuint getHandle() { return _ibh; }

protected:

    virtual void set(const void* vertexData, unsigned int vertexCount, unsigned int vertexStart) override;

    GLuint _ibh;
};


}

#endif // BGFX_INDEXBUFFER_H_
