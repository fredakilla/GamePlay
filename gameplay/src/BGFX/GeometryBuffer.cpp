#include "../BGFX/GeometryBuffer.h"

namespace gameplay
{

GeometryBuffer::GeometryBuffer() :
    _elementSize(0)
    , _elementCount(0)
    , _elementStart(0)
    , _dynamic(false)
    , _lockState(LOCK_NONE)
    , _lockStart(0)
    , _lockCount(0)
    , _lockScratchData(nullptr)
{
}

GeometryBuffer::~GeometryBuffer()
{
    _memoryBuffer.destroy();
}

void GeometryBuffer::initialize(uint32_t elementSize, uint32_t elementCount, bool dynamic)
{
    _elementSize = elementSize;
    _elementCount = elementCount;
    _dynamic = dynamic;
    _elementStart = 0;

    // allocate memory buffer
    uint32_t memSize = elementSize * elementCount;
    _memoryBuffer.create(memSize);
}

void GeometryBuffer::set(const void* data, uint32_t count, uint32_t start)
{
    bool needResize = _elementCount < count;

    _elementCount = count;
    _elementStart = start;

    if(_dynamic)
    {
        // need to resize buffer ?
        if(needResize)
        {
            uint32_t newSize = _elementSize * _elementCount;
            if(newSize > _memoryBuffer.getSize())
                _memoryBuffer.resize(newSize);
        }
    }

    // copy vertex data into memory buffer
    if(data)
    {
        uint32_t memSize = _elementSize * count;
        GP_ASSERT(memSize <= _memoryBuffer.getSize());
        memcpy(_memoryBuffer.map(0), data, memSize);
    }
}

void GeometryBuffer::bind()
{
}

void * GeometryBuffer::lock(uint32_t start, uint32_t count)
{
    if (_lockState != LOCK_NONE)
    {
        GP_ERROR("Buffer already locked.");
        return nullptr;
    }

    if (!count)
        return nullptr;

    GP_ASSERT(_memoryBuffer.getSize() >= _elementSize * (start+count) );

    _lockStart = start;
    _lockCount = count;
    _lockState = LOCK_ACTIVE;
    _lockScratchData = _memoryBuffer.map(0);
    GP_ASSERT(_lockScratchData);

    return _lockScratchData;
}

void GeometryBuffer::unLock()
{
    if (_lockState == LOCK_ACTIVE)
    {
        _memoryBuffer.unmap();
        _lockState = LOCK_NONE;
        _lockScratchData = nullptr;
    }
    else
    {
        GP_ERROR("Buffer was not locked.");
    }
}

} // end namespace gameplay
