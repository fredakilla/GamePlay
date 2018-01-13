#ifndef BGFX_VERTEXBUFFER_H_
#define BGFX_VERTEXBUFFER_H_

#include "VertexBuffer.h"
#include "MemoryBuffer.h"
#include "GeometryBuffer.h"

namespace gameplay {


class BGFXVertexBuffer : public VertexBuffer
{
public:
    BGFXVertexBuffer(const VertexFormat& vertexFormat, unsigned int vertexCount, bool dynamic);
    virtual ~BGFXVertexBuffer();
    void set(const void* data, unsigned int count, unsigned int start);
    void bind();
    void * lock(unsigned start, unsigned count, bool discard=false);
    void unLock();





    static void createVertexDecl(const VertexFormat &vertexFormat, bgfx::VertexDecl &vertexDecl);

    void createStaticBuffer();
    void createDynamicBuffer();

    bgfx::VertexBufferHandle            _svbh;          // static vertex buffer handle
    bgfx::DynamicVertexBufferHandle     _dvbh;          // dynamic vertex buffer handle
    bgfx::VertexDecl                    _vertexDecl;    // vertex declaration


};


#if 0
class BGFXVertexBuffer : public GeometryBuffer //VertexBuffer
{
public:
    BGFXVertexBuffer(const VertexFormat& vertexFormat, unsigned int vertexCount, bool dynamic);
    ~BGFXVertexBuffer();
    void bind() override;


    void * lock(unsigned start, unsigned count, bool discard=false) override;
    void unLock() override;

    static void createVertexDecl(const VertexFormat &vertexFormat, bgfx::VertexDecl &vertexDecl);

protected:


    virtual void set(const void* vertexData, unsigned int vertexCount, unsigned int vertexStart) override;

private:

    void createStaticBuffer();
    void createDynamicBuffer();

    bgfx::VertexBufferHandle            _svbh;          // static vertex buffer handle
    bgfx::DynamicVertexBufferHandle     _dvbh;          // dynamic vertex buffer handle
    bgfx::VertexDecl                    _vertexDecl;    // vertex declaration



    LockState _lockState;
    unsigned int _lockStart;
    unsigned int _lockCount;
    void* _lockScratchData;

    MemoryBuffer _memoryBuffer;
};
#endif

} // end namespace gameplay

#endif // BGFX_VERTEXBUFFER_H_
