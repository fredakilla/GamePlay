#pragma once

#include "../Base.h"
#include "../VertexFormat.h"
#include "../BGFX/GeometryBuffer.h"

namespace gameplay
{

class BGFXVertexBuffer : public GeometryBuffer
{
public:
    BGFXVertexBuffer(const VertexFormat& vertexFormat, uint32_t vertexCount, bool dynamic);
    virtual ~BGFXVertexBuffer();

    static void createVertexDecl(const VertexFormat &vertexFormat, bgfx::VertexDecl &vertexDecl);
    const bgfx::VertexDecl getVertexDecl() const { return _vertexDecl; }

    void set(const void* data, uint32_t count, uint32_t start) override;
    void bind() override;
    void* lock(uint32_t start, uint32_t count) override;
    void unLock() override;

private:
    void createStaticBuffer();
    void createDynamicBuffer();

    bgfx::VertexBufferHandle _svbh;             // static vertex buffer handle
    bgfx::DynamicVertexBufferHandle _dvbh;      // dynamic vertex buffer handle
    bgfx::VertexDecl _vertexDecl;               // vertex declaration
};

} // end namespace gameplay
