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
    virtual void bind() = 0;

    bool isDynamic() { return _dynamic; }


protected:
    unsigned int _indexFormat;
    unsigned int _indexCount;
    unsigned int _indexStart;
    bool _dynamic;
    unsigned int _indexSize;
};

}

#endif // INDEXBUFFER_H_
