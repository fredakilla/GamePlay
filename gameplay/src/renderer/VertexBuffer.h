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
    virtual void bind() = 0;


    virtual void * lock(unsigned start, unsigned count, bool discard=false) {}
    virtual void unLock() {}

    bool isDynamic() { return _dynamic; }


    const unsigned int getVertexCount() const { return _vertexCount; }


protected:
    VertexFormat * _vertexFormat;
    unsigned int _vertexCount;
    unsigned int _vertexStart;
    bool _dynamic;

};


}

#endif  // VERTEXBUFFER_H_
