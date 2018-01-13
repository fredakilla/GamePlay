#include "BGFXIndexBuffer.h"
#include "Mesh.h"

namespace gameplay {

BGFXIndexBuffer::BGFXIndexBuffer(const unsigned int indexFormat, unsigned int indexCount, bool dynamic)
{
    _indexFormat = indexFormat;
    _indexCount = indexCount;
    _dynamic = dynamic;

    _sibh = BGFX_INVALID_HANDLE;
    _dibh = BGFX_INVALID_HANDLE;


    switch (indexFormat)
    {
    case Mesh::INDEX8:
        _indexSize = 1;
        break;
    case Mesh::INDEX16:
        _indexSize = 2;
        break;
    case Mesh::INDEX32:
        _indexSize = 4;
        break;
    default:
        GP_ERROR("Unsupported index format (%d).", indexFormat);
    }


    // allocate memory buffer
    unsigned int memSize = _indexSize * _indexCount;
    _memoryBuffer.create(memSize);

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

    _dibh = bgfx::createDynamicIndexBuffer(_indexCount, flags);
    GP_ASSERT(bgfx::isValid(_dibh));
}

void BGFXIndexBuffer::createStaticBuffer()
{
    GP_ASSERT(!_dynamic && !bgfx::isValid(_sibh));
    GP_ASSERT(_memoryBuffer.getSize() > 0);

    void * dataPtr = _memoryBuffer.map(0);
    GP_ASSERT(dataPtr);
    const bgfx::Memory * mem = bgfx::makeRef(dataPtr, _memoryBuffer.getSize());


    uint16_t flags = BGFX_BUFFER_NONE;
    if(_indexFormat == Mesh::INDEX32)
        flags |= BGFX_BUFFER_INDEX32;

    _sibh = bgfx::createIndexBuffer(mem, flags);
    GP_ASSERT(bgfx::isValid(_sibh));
}



void BGFXIndexBuffer::set(const void* indexData, unsigned int indexCount, unsigned int indexStart)
{
    /*if(indexCount == 0)
    {
        GP_WARN("BGFXIndexBuffer::set() - indexCount is null.");
        return;
    }

    uint32_t size = _indexSize * indexCount;
    const bgfx::Memory* mem = bgfx::copy(indexData, size);
    uint16_t flags = BGFX_BUFFER_NONE;

    if(_indexFormat == Mesh::INDEX32)
        flags |= BGFX_BUFFER_INDEX32;

    if(_dynamic)
    {
        if(!bgfx::isValid(_dibh))
        {
            flags |= BGFX_BUFFER_ALLOW_RESIZE;
            _dibh = bgfx::createDynamicIndexBuffer(indexCount, flags);
        }

        GP_ASSERT(bgfx::isValid(_dibh));
        bgfx::updateDynamicIndexBuffer(_dibh, indexStart, mem);
    }
    else
    {
        if(!bgfx::isValid(_sibh))
        {
            _sibh = bgfx::createIndexBuffer(mem, flags);
            GP_ASSERT(bgfx::isValid(_sibh));
        }
        else
        {
            GP_WARN("BGFXIndexBuffer::set() - static index buffer already set.");
        }
    }*/





   /* GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibh) );

        unsigned int indexSize = 0;
        switch (_indexFormat)
        {
        case Mesh::INDEX8:
            indexSize = 1;
            break;
        case Mesh::INDEX16:
            indexSize = 2;
            break;
        case Mesh::INDEX32:
            indexSize = 4;
            break;
        default:
            GP_ERROR("Unsupported index format (%d).", _indexFormat);
            return;
        }

        if (indexStart == 0 && indexCount == 0)
        {
            GL_ASSERT( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * _indexCount, indexData, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
        }
        else
        {
            if (indexCount == 0)
            {
                indexCount = _indexCount - indexStart;
            }

            GL_ASSERT( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexStart * indexSize, indexCount * indexSize, indexData) );
        }*/




    if (indexStart == 0 && indexCount == 0)
    {
        indexCount = _indexCount;

    }
    else
    {
        if (indexCount == 0)
        {
            indexCount = _indexCount - indexStart;
        }
        else
        {
            indexCount =  indexStart + indexCount;
        }
  }






    if(_dynamic)
    {
        // need to resize buffer ?
        //if(_vertexCount < (vertexStart + vertexCount))
        if(_indexCount < indexCount)
        {
            uint32_t newSize = indexCount * _indexSize;
            if(newSize > _memoryBuffer.getSize())
                _memoryBuffer.resize(newSize);

            //_vertexCount = vertexStart + vertexCount;
            //vertexCount = vertexStart + vertexCount;
        }
    }
    else
    {
        // create static bgfx buffer
        if(!bgfx::isValid(_sibh))
            createStaticBuffer();
    }


    if(indexData)
    {
        // copy vertex data to memory buffer
        uint32_t memSize = indexCount * _indexSize;
        GP_ASSERT(memSize <= _memoryBuffer.getSize());

        memcpy(_memoryBuffer.map(0), indexData, memSize);

        // if dynamic, update vertex buffer with memory buffer
        if(_dynamic)
        {
            GP_ASSERT(bgfx::isValid(_dibh));

            const bgfx::Memory* mem = bgfx::makeRef(_memoryBuffer.map(0), memSize);
            bgfx::updateDynamicIndexBuffer(_dibh, indexStart, mem);
        }
    }


    _indexStart = indexStart;
    _indexCount = indexCount;




}

void BGFXIndexBuffer::bind()
{
    if(_dynamic)
    {
        GP_ASSERT(bgfx::isValid(_dibh));
        bgfx::setIndexBuffer(_dibh, _indexStart, _indexCount);
    }
    else
    {
        GP_ASSERT(bgfx::isValid(_sibh));
        bgfx::setIndexBuffer(_sibh, _indexStart, _indexCount);
    }


    /*if(_dynamic)
    {
        if(bgfx::isValid(_dibh))
            bgfx::setIndexBuffer(_dibh);
        else
            GP_WARN("BGFXIndexBuffer::bind() - dynamic index buffer no set.");

    }
    else
    {
        if(bgfx::isValid(_sibh))
            bgfx::setIndexBuffer(_sibh);
        else
            GP_WARN("BGFXIndexBuffer::bind() - static index buffer no set.");

    }*/
}


} // end namespace gameplay
