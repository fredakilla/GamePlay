#include "../BGFX/BGFXIndexBuffer.h"
#include "../Mesh.h"

namespace gameplay {

BGFXIndexBuffer::BGFXIndexBuffer(const Mesh::IndexFormat indexFormat, unsigned int indexCount, bool dynamic)
{
    _sibh = BGFX_INVALID_HANDLE;
    _dibh = BGFX_INVALID_HANDLE;

    _indexFormat = indexFormat;
    switch (indexFormat)
    {
    case Mesh::INDEX8:
        _elementSize = 1;
        break;
    case Mesh::INDEX16:
        _elementSize = 2;
        break;
    case Mesh::INDEX32:
        _elementSize = 4;
        break;
    default:
        GP_ERROR("Unsupported index format (%d).", indexFormat);
    }

    // initialise Geometry buffer.
    initialize(_elementSize, indexCount, dynamic);

    // if dynamic, create bgfx vertex buffer here.
    // if static, creation will be delayed when setting vertice data.
    if(_dynamic)
    {
        createDynamicBuffer();
    }
}

BGFXIndexBuffer::~BGFXIndexBuffer()
{
    if(_dynamic)
    {
        if(bgfx::isValid(_dibh))
            bgfx::destroy(_dibh);
    }
    else
    {
        if(bgfx::isValid(_sibh))
            bgfx::destroy(_sibh);
    }
}

void BGFXIndexBuffer::createDynamicBuffer()
{
    GP_ASSERT(_dynamic);

    uint16_t flags = /*BGFX_BUFFER_NONE; //*/BGFX_BUFFER_ALLOW_RESIZE;
    if(_indexFormat == Mesh::INDEX32)
        flags |= BGFX_BUFFER_INDEX32;

    _dibh = bgfx::createDynamicIndexBuffer(_elementCount, flags);
    GP_ASSERT(bgfx::isValid(_dibh));
}

void BGFXIndexBuffer::createStaticBuffer()
{
    GP_ASSERT(!_dynamic && !bgfx::isValid(_sibh));
    GP_ASSERT(_memoryBuffer.getSize() > 0);

    void * dataPtr = _memoryBuffer.map(0);
    GP_ASSERT(dataPtr);
    const bgfx::Memory * mem = bgfx::makeRef(dataPtr, _memoryBuffer.getSize());

    uint16_t flags = /*BGFX_BUFFER_NONE; //*/BGFX_BUFFER_ALLOW_RESIZE;
    if(_indexFormat == Mesh::INDEX32)
        flags |= BGFX_BUFFER_INDEX32;

    _sibh = bgfx::createIndexBuffer(mem, flags);
    GP_ASSERT(bgfx::isValid(_sibh));
}

void BGFXIndexBuffer::set(const void* data, unsigned int count, unsigned int start)
{
    GeometryBuffer::set(data, count, start);

    if(_dynamic)
    {
        // if dynamic, update vertex buffer with memory buffer
        GP_ASSERT(bgfx::isValid(_dibh));
        const bgfx::Memory* mem = bgfx::makeRef(_memoryBuffer.map(0), _memoryBuffer.getSize());
        bgfx::updateDynamicIndexBuffer(_dibh, _elementStart, mem);

    }
    else
    {
        // create static bgfx buffer
        if(!bgfx::isValid(_sibh))
            createStaticBuffer();
    }

}

void BGFXIndexBuffer::bind()
{
    if(_dynamic)
    {
        GP_ASSERT(bgfx::isValid(_dibh));
        bgfx::setIndexBuffer(_dibh, _elementStart, _elementCount);
    }
    else
    {
        GP_ASSERT(bgfx::isValid(_sibh));
        bgfx::setIndexBuffer(_sibh, _elementStart, _elementCount);
    }
}

void * BGFXIndexBuffer::lock(unsigned start, unsigned count, bool discard)
{
    GP_ASSERT(_dynamic && bgfx::isValid(_dibh));

    return GeometryBuffer::lock(start, count, discard);
}

void BGFXIndexBuffer::unLock()
{
    GP_ASSERT(_dynamic && bgfx::isValid(_dibh));

    if (_lockState == LOCK_ACTIVE)
    {
        bgfx::updateDynamicIndexBuffer(_dibh, _lockStart, bgfx::makeRef(_lockScratchData, _memoryBuffer.getSize()) );
        GeometryBuffer::unLock();
    }
}

} // end namespace gameplay
