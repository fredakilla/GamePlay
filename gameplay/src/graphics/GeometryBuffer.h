#ifndef GEOMETRYBUFFER_H
#define GEOMETRYBUFFER_H

#include "MemoryBuffer.h"

namespace gameplay
{

class GeometryBuffer
{

public:
    GeometryBuffer();//unsigned int elementSize, unsigned int elementCount, bool dynamic);
    virtual ~GeometryBuffer();
    void create(unsigned int elementSize, unsigned int elementCount, bool dynamic);
    virtual void set(const void* data, unsigned int count, unsigned int start);
    virtual void bind();
    virtual void * lock(unsigned start, unsigned count, bool discard=false);
    virtual void unLock();

    bool isDynamic() { return _dynamic; }
    const unsigned int getElementCount() const { return _elementCount; }


protected:
    unsigned int _elementSize;
    unsigned int _elementCount;
    unsigned int _elementStart;
    bool _dynamic;
    MemoryBuffer _memoryBuffer;


    LockState _lockState;
    unsigned int _lockStart;
    unsigned int _lockCount;
    void* _lockScratchData;
};

}

#endif // GEOMETRYBUFFER_H
