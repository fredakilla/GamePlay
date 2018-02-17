#include "../Base.h"
#include "../BGFX/GeometryBuffer.h"

namespace gameplay {

GeometryBuffer::GeometryBuffer()
{
    _elementSize = 0;
    _elementCount = 0;
    _dynamic = false;
    _elementStart = 0;

    _lockState = LOCK_NONE;
    _lockStart = 0;
    _lockCount = 0;
    _lockScratchData = nullptr;
}

GeometryBuffer::~GeometryBuffer()
{
    _memoryBuffer.destroy();
}

void GeometryBuffer::initialize(unsigned int elementSize, unsigned int elementCount, bool dynamic)
{
    _elementSize = elementSize;
    _elementCount = elementCount;
    _dynamic = dynamic;
    _elementStart = 0;

    // allocate memory buffer
    unsigned int memSize = elementSize * elementCount;
    _memoryBuffer.create(memSize);
}



void GeometryBuffer::set(const void* data, unsigned int count, unsigned int start)
{
    bool needResize = false;

    if(_elementCount < count)
        needResize = true;

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

    // copy vertex data to memory buffer
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

void * GeometryBuffer::lock(unsigned start, unsigned count, bool discard)
{
    if (_lockState != LOCK_NONE)
    {
        GP_ERROR("Vertex buffer already locked");
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
        GP_ERROR("GeometryBuffer::unLock() - buffer was not locked.");
    }
}

} // end namespace gameplay
