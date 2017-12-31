#ifndef INDEXBUFFER_H_
#define INDEXBUFFER_H_

#include "Base.h"

namespace gameplay
{

class IndexBuffer
{
public:
    virtual ~IndexBuffer() {}
    virtual void set(const void* indexData, unsigned int indexCount, unsigned int indexStart) = 0;
    virtual GLuint getHandle() {}
    virtual void bind() = 0;

protected:
    unsigned int _indexFormat;
    unsigned int _indexCount;
    bool _dynamic;
    unsigned int _indexSize;
};

}

#endif // INDEXBUFFER_H_
