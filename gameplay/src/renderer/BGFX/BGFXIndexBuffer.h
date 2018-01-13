#ifndef BGFX_INDEXBUFFER_H_
#define BGFX_INDEXBUFFER_H_

#include "IndexBuffer.h"

namespace gameplay {


class BGFXIndexBuffer : public IndexBuffer
{
public:
    BGFXIndexBuffer(const unsigned int indexFormat, unsigned int indexCount, bool dynamic);
    virtual ~BGFXIndexBuffer();
    void set(const void* data, unsigned int count, unsigned int start);
    void bind();
    void * lock(unsigned start, unsigned count, bool discard=false);
    void unLock();


private:

    void createStaticBuffer();
    void createDynamicBuffer();

    bgfx::IndexBufferHandle         _sibh;  // static index buffer handle
    bgfx::DynamicIndexBufferHandle  _dibh;  // dynamic index buffer handle
};








#if 0
class BGFXIndexBuffer : public IndexBuffer
{
public:
    BGFXIndexBuffer(const unsigned int indexFormat, unsigned int indexCount, bool dynamic);
    ~BGFXIndexBuffer();
    void bind() override;

protected:

    virtual void set(const void* vertexData, unsigned int vertexCount, unsigned int vertexStart) override;

private:

    void createStaticBuffer();
    void createDynamicBuffer();



    MemoryBuffer _memoryBuffer;

    bgfx::IndexBufferHandle         _sibh;  // static index buffer handle
    bgfx::DynamicIndexBufferHandle  _dibh;  // dynamic index buffer handle
};
#endif

} // end namespace gameplay

#endif // BGFX_INDEXBUFFER_H_
