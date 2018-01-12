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








class IBuffer
{
public:
    IBuffer();
    virtual ~IBuffer();

    void resize(int newSize)
    {
        if (_sizeMax < newSize)
            create(newSize);
        _size = newSize;
    }
    virtual void create(int newSize);
    virtual void destroy();
    virtual void* map(int stride) { return nullptr; }
    virtual void unmap() {}

    int getSize() { return _size; }
    int getSizeMax() { return _sizeMax; }

protected:
    int _sizeMax;
    int _size;
};

class MemoryBuffer : public IBuffer
{
public:

    MemoryBuffer();
    virtual ~MemoryBuffer();
    virtual void create(int newSize);
    virtual void destroy();
    virtual void* map(int stride) { return buffer; }

private:
    char* buffer;
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
    //void * _vertexData;

    MemoryBuffer _memoryBuffer;
};

} // end namespace gameplay

#endif // BGFX_VERTEXBUFFER_H_
