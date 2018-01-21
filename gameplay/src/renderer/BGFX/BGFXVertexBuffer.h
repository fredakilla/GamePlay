#ifndef BGFX_VERTEXBUFFER_H_
#define BGFX_VERTEXBUFFER_H_

#include "../../core/Base.h"
#include "../../graphics/VertexFormat.h"
#include "../../graphics/GeometryBuffer.h"

namespace gameplay
{

class BGFXVertexBuffer : public GeometryBuffer
{
public:
    BGFXVertexBuffer(const VertexFormat& vertexFormat, unsigned int vertexCount, bool dynamic);
    virtual ~BGFXVertexBuffer();
    void set(const void* data, unsigned int count, unsigned int start);
    void bind();
    void * lock(unsigned start, unsigned count, bool discard=false);
    void unLock();

    static void createVertexDecl(const VertexFormat &vertexFormat, bgfx::VertexDecl &vertexDecl);


    const bgfx::VertexDecl getVertexDecl() const { return _vertexDecl; }

private:
    void createStaticBuffer();
    void createDynamicBuffer();

    bgfx::VertexBufferHandle            _svbh;          // static vertex buffer handle
    bgfx::DynamicVertexBufferHandle     _dvbh;          // dynamic vertex buffer handle
    bgfx::VertexDecl                    _vertexDecl;    // vertex declaration

};

} // end namespace gameplay

#endif // BGFX_VERTEXBUFFER_H_
