#ifndef BGFX_INDEXBUFFER_H_
#define BGFX_INDEXBUFFER_H_

#include "IndexBuffer.h"

namespace gameplay {

class BGFXIndexBuffer : public IndexBuffer
{
public:
    BGFXIndexBuffer(const unsigned int indexFormat, unsigned int indexCount, bool dynamic);
    ~BGFXIndexBuffer();
    //GLuint getHandle() { return _ibh; }
    void bind() override;

protected:

    virtual void set(const void* vertexData, unsigned int vertexCount, unsigned int vertexStart) override;


    union
    {
        bgfx::IndexBufferHandle        _sibh;
        bgfx::DynamicIndexBufferHandle _dibh;
    };


};


} // end namespace gameplay

#endif // BGFX_INDEXBUFFER_H_
