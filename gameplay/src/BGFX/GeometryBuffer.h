#ifndef GEOMETRYBUFFER_H
#define GEOMETRYBUFFER_H

#include "../BGFX/MemoryBuffer.h"

namespace gameplay
{

class GeometryBuffer
{
public:
    GeometryBuffer();
    virtual ~GeometryBuffer();
    virtual void set(const void* data, unsigned int count, unsigned int start);
    virtual void bind();
    virtual void * lock(unsigned start, unsigned count, bool discard=false);
    virtual void unLock();
    const unsigned int getElementCount() const { return _elementCount; }
    bool isDynamic() { return _dynamic; }

protected:
    void initialize(unsigned int elementSize, unsigned int elementCount, bool dynamic);

protected:
    unsigned int    _elementSize;
    unsigned int    _elementCount;
    unsigned int    _elementStart;
    MemoryBuffer    _memoryBuffer;
    bool            _dynamic;

    LockState       _lockState;
    unsigned int    _lockStart;
    unsigned int    _lockCount;
    void*           _lockScratchData;
};

}

#endif // GEOMETRYBUFFER_H
