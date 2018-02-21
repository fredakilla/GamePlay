#pragma once

#include "../BGFX/MemoryBuffer.h"

namespace gameplay
{

class GeometryBuffer
{
public:
    enum LockState
    {
        LOCK_NONE = 0,
        LOCK_ACTIVE,
    };

    GeometryBuffer();
    virtual ~GeometryBuffer();
    virtual void set(const void* data, uint32_t count, uint32_t start);
    virtual void bind();
    virtual void* lock(uint32_t start, uint32_t count);
    virtual void unLock();
    const uint32_t getElementCount() const { return _elementCount; }
    bool isDynamic() { return _dynamic; }

protected:
    void initialize(uint32_t elementSize, uint32_t elementCount, bool dynamic);

protected:
    uint32_t _elementSize;
    uint32_t _elementCount;
    uint32_t _elementStart;
    MemoryBuffer _memoryBuffer;
    bool _dynamic;
    LockState _lockState;
    uint32_t _lockStart;
    uint32_t _lockCount;
    void* _lockScratchData;
};

}
