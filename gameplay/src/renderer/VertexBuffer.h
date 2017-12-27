#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#include "Base.h"
#include "VertexFormat.h"

namespace gameplay
{

class VertexBuffer
{
public:
    virtual ~VertexBuffer() {}
    virtual void set(const void* vertexData, unsigned int vertexCount, unsigned int vertexStart) = 0;
    virtual GLuint getHandle() {}
    virtual void bind() = 0;

protected:
    VertexFormat * _vertexFormat;
    unsigned int _vertexCount;
    bool _dynamic;

};


}

#endif  // VERTEXBUFFER_H_
