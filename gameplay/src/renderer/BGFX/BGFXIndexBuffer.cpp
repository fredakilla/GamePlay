#include "BGFXIndexBuffer.h"
#include "Mesh.h"

namespace gameplay {

BGFXIndexBuffer::BGFXIndexBuffer(const unsigned int indexFormat, unsigned int indexCount, bool dynamic)
{
    _indexFormat = indexFormat;
    _indexCount = indexCount;
    _dynamic = dynamic;

    _sibh = BGFX_INVALID_HANDLE;

    //GL_ASSERT( glGenBuffers(1, &_ibh) );
    //GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibh) );
    //
    unsigned int indexSize = 0;
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
        //glDeleteBuffers(1, &_ibh);
    }
    //
    //GL_ASSERT( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * indexCount, NULL, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
}

BGFXIndexBuffer::~BGFXIndexBuffer()
{
    if(bgfx::isValid(_sibh))
        bgfx::destroy(_sibh);
}

void BGFXIndexBuffer::set(const void* indexData, unsigned int indexCount, unsigned int indexStart)
{
    if(indexCount == 0)
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
            GP_ASSERT(bgfx::isValid(_dibh));
        }
        else
        {
            GP_WARN("BGFXIndexBuffer::set() - static index buffer already set.");
        }
    }





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
}

void BGFXIndexBuffer::bind()
{
    if(_dynamic)
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

    }
}


} // end namespace gameplay
