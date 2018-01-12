#ifndef BGFX_VERTEXBUFFER_H_
#define BGFX_VERTEXBUFFER_H_

#include "VertexBuffer.h"

namespace gameplay {


/// Vertex/index buffer lock state.
enum LockState
{
    LOCK_NONE = 0,
    LOCK_HARDWARE,
    LOCK_SHADOW,
    LOCK_SCRATCH
};























class BGFXVertexBuffer : public VertexBuffer
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

    bgfx::VertexBufferHandle            _svbh;          // static vertex buffer handle
    bgfx::DynamicVertexBufferHandle     _dvbh;          // dynamic vertex buffer handle
    bgfx::VertexDecl                    _vertexDecl;    // vertex declaration



    LockState _lockState;
    unsigned int _lockStart;
    unsigned int _lockCount;
    void* _lockScratchData;
    //const bgfx::Memory * _vertexData;
    void * _vertexData;
};

} // end namespace gameplay

#endif // BGFX_VERTEXBUFFER_H_
