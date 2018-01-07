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



    union
    {
        bgfx::VertexBufferHandle        _svbh;
        bgfx::DynamicVertexBufferHandle _dvbh;
    };

    bgfx::VertexDecl _vertexDecl;
};

} // end namespace gameplay

#endif // BGFX_VERTEXBUFFER_H_
