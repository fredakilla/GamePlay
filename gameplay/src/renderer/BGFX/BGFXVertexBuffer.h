#ifndef BGFX_VERTEXBUFFER_H_
#define BGFX_VERTEXBUFFER_H_

#include "VertexBuffer.h"

namespace gameplay {

class BGFXVertexBuffer : public VertexBuffer
{
public:
    BGFXVertexBuffer(const VertexFormat& vertexFormat, unsigned int vertexCount, bool dynamic);
    ~BGFXVertexBuffer();
    //GLuint getHandle() override { return _vbh; }
    void bind() override;

    static void createVertexDecl(const VertexFormat &vertexFormat, bgfx::VertexDecl &vertexDecl);

protected:

    virtual void set(const void* vertexData, unsigned int vertexCount, unsigned int vertexStart) override;

private:

    bgfx::VertexBufferHandle            _svbh;          // static vertex buffer handle
    bgfx::DynamicVertexBufferHandle     _dvbh;          // dynamic vertex buffer handle
    bgfx::VertexDecl                    _vertexDecl;    // vertex declaration
};

} // end namespace gameplay

#endif // BGFX_VERTEXBUFFER_H_
