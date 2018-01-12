#ifndef BGFX_INDEXBUFFER_H_
#define BGFX_INDEXBUFFER_H_

#include "IndexBuffer.h"

namespace gameplay {

class BGFXIndexBuffer : public IndexBuffer
{
public:
    BGFXIndexBuffer(const unsigned int indexFormat, unsigned int indexCount, bool dynamic);
    ~BGFXIndexBuffer();
    void bind() override;

protected:

    virtual void set(const void* vertexData, unsigned int vertexCount, unsigned int vertexStart) override;

    bgfx::IndexBufferHandle         _sibh;  // static index buffer handle
    bgfx::DynamicIndexBufferHandle  _dibh;  // dynamic index buffer handle
};


} // end namespace gameplay

#endif // BGFX_INDEXBUFFER_H_
