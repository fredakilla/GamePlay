#ifndef BGFX_INDEXBUFFER_H_
#define BGFX_INDEXBUFFER_H_

#include "../BGFX/GeometryBuffer.h"
#include "../Base.h"
#include "../Mesh.h"

namespace gameplay
{

class BGFXIndexBuffer : public GeometryBuffer
{
public:
    BGFXIndexBuffer(const Mesh::IndexFormat indexFormat, unsigned int indexCount, bool dynamic);
    virtual ~BGFXIndexBuffer();
    void set(const void* data, unsigned int count, unsigned int start);
    void bind();
    void * lock(unsigned start, unsigned count, bool discard=false);
    void unLock();

private:
    void createStaticBuffer();
    void createDynamicBuffer();

    bgfx::IndexBufferHandle         _sibh;          // static index buffer handle
    bgfx::DynamicIndexBufferHandle  _dibh;          // dynamic index buffer handle
    Mesh::IndexFormat               _indexFormat;   // 8, 16 or 32 bits
};

} // end namespace gameplay

#endif // BGFX_INDEXBUFFER_H_
