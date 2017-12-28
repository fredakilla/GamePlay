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

protected:


    virtual void set(const void* vertexData, unsigned int vertexCount, unsigned int vertexStart) override;



private:

    void _createVertexDecl(const VertexFormat &vertexFormat);

    union
    {
        bgfx::VertexBufferHandle        _svbh;
        bgfx::DynamicIndexBufferHandle  _dvbh;
    };

    bgfx::VertexDecl _vertexDecl;
};

}

#endif // BGFX_VERTEXBUFFER_H_
