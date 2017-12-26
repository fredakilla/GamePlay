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

protected:
    unsigned int _indexFormat;
    unsigned int _indexCount;
    bool _dynamic;

};

}

#endif // INDEXBUFFER_H_
