#ifndef GL2_INDEXBUFFER_H_
#define GL2_INDEXBUFFER_H_

#include "IndexBuffer.h"

namespace gameplay {

class GL2IndexBuffer : public IndexBuffer
{
public:
    GL2IndexBuffer(const unsigned int indexFormat, unsigned int indexCount, bool dynamic);
    ~GL2IndexBuffer();
    virtual GLuint getHandle() { return _ibh; }

protected:

    virtual void set(const void* vertexData, unsigned int vertexCount, unsigned int vertexStart) override;

    GLuint _ibh;
};


}

#endif // GL2_INDEXBUFFER_H_
