#include "Base.h"
#include "GeometryBuffer.h"


namespace gameplay
{

/*GeometryBuffer::GeometryBuffer(unsigned int elementSize, unsigned int elementCount, bool dynamic)
{
    _elementSize = elementSize;
    _elementCount = elementCount;
    _dynamic = dynamic;
    _elementStart = 0;

    // allocate memory buffer
    unsigned int memSize = elementSize * elementCount;
    _memoryBuffer.create(memSize);
}*/

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

void GeometryBuffer::create(unsigned int elementSize, unsigned int elementCount, bool dynamic)
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
    //GP_ASSERT( _elementCount >= (count + start) );

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


    if(data)
    {
        // copy vertex data to memory buffer
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
#if 1
    if (_lockState != LOCK_NONE)
    {
        GP_ERROR("Vertex buffer already locked");
        return nullptr;
    }

    if (!count)
        return nullptr;

    /*if (!_vertexDecl)
    {
        GP_ERROR("Vertex elements not defined, can not lock vertex buffer");
        return nullptr;
    }*/

    /*if (start + count > _vertexCount)
    {
        GP_ERROR("Illegal range for locking vertex buffer");
        return nullptr;

        // don't resize static
        GP_ASSERT(_dynamic);

        // resize buffer
        _memoryBuffer.resize(_vertexDecl.getSize(start + count));
    }*/

    int debug1 = start+count;
    int debug2 = _elementSize * (start+count);
    int debug3 = _memoryBuffer.getSize();
    GP_ASSERT(_memoryBuffer.getSize() >= _elementSize * (start+count) );


    _lockStart = start;
    _lockCount = count;
    _lockState = LOCK_SCRATCH;
    //_lockScratchData = bx::mem graphics_->ReserveScratchBuffer(count * vertexSize_);
   // _lockScratchData = bgfx::makeRef()
    //const bgfx::Memory* mem = bgfx::alloc()

    _lockScratchData = _memoryBuffer.map(0);

    GP_ASSERT(_lockScratchData);

    return _lockScratchData;
#endif
}

void GeometryBuffer::unLock()
{
   // if (_lockState == LOCK_SCRATCH)
    {
        //bgfx::updateDynamicVertexBuffer(_dvbh, _lockStart, bgfx::makeRef(_lockScratchData, _vertexDecl.getSize(_vertexCount)) );
        //bgfx::updateDynamicVertexBuffer(_dvbh, _lockStart, bgfx::makeRef(_lockScratchData, _memoryBuffer.getSize()) );


        _memoryBuffer.unmap();

        //setDataRange(shadowData_.Get() + lockStart_ * vertexSize_, lockStart_, lockCount_, discardLock_);
        _lockState = LOCK_NONE;
        _lockScratchData = nullptr;
    }
}

} // end namespace gameplay
