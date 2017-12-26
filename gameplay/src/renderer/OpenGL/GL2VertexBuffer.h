#ifndef GL2_VERTEXBUFFER_H_
#define GL2_VERTEXBUFFER_H_

#include "VertexBuffer.h"

namespace gameplay {

class GL2VertexBuffer : public VertexBuffer
{
public:
    GL2VertexBuffer(const VertexFormat& vertexFormat, unsigned int vertexCount, bool dynamic);
    ~GL2VertexBuffer();
    virtual GLuint getHandle() { return _vbh; }

protected:

    virtual void set(const void* vertexData, unsigned int vertexCount, unsigned int vertexStart) override;

    GLuint _vbh;
};

}

#endif // GL2_VERTEXBUFFER_H_
