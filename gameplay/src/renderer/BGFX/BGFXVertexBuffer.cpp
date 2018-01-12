#include "BGFXVertexBuffer.h"

namespace gameplay {




IBuffer::IBuffer()
{
    _sizeMax = 0;
    _size = 0;
}

IBuffer::~IBuffer()
{
    destroy();
}

void IBuffer::create(int newSize)
{
    destroy();
    _sizeMax = newSize;
}

void IBuffer::destroy()
{
    _sizeMax = 0;
    _size = 0;
}

// --------------------------------

MemoryBuffer::MemoryBuffer() : IBuffer()
{
    buffer = nullptr;
}

MemoryBuffer::~MemoryBuffer()
{
    destroy();
}

void MemoryBuffer::create(int newSize)
{
    IBuffer::create(newSize);
    buffer = new char[newSize];
}

void MemoryBuffer::destroy()
{
    IBuffer::destroy();

    if (buffer)
    {
        delete[] buffer;
        buffer = nullptr;
    }
}

















BGFXVertexBuffer::BGFXVertexBuffer(const VertexFormat &vertexFormat, unsigned int vertexCount, bool dynamic)
{
    _vertexFormat = (VertexFormat*)&vertexFormat;
    _vertexCount = vertexCount;
    _dynamic = dynamic;

    createVertexDecl(vertexFormat, _vertexDecl);

    _svbh = BGFX_INVALID_HANDLE;
    _dvbh = BGFX_INVALID_HANDLE;

    _lockState = LOCK_NONE;




    unsigned int memSize = _vertexDecl.getSize(_vertexCount);
    _memoryBuffer.create(memSize);

    if(_dynamic)
    {
        uint16_t flags = BGFX_BUFFER_NONE; //BGFX_BUFFER_ALLOW_RESIZE;
        _dvbh = bgfx::createDynamicVertexBuffer(_vertexCount, _vertexDecl, flags);
        GP_ASSERT(bgfx::isValid(_dvbh));
    }


}





BGFXVertexBuffer::~BGFXVertexBuffer()
{
    if(_dynamic)
    {
        if(bgfx::isValid(_dvbh))
            bgfx::destroy(_dvbh);
    }
    else
    {
        if(bgfx::isValid(_svbh))
            bgfx::destroy(_svbh);
    }

    _memoryBuffer.destroy();
}


void getBgfxAttribute(const VertexFormat::Element& element, bgfx::Attrib::Enum& attrib)
{
    switch(element.usage)
    {
        case VertexFormat::POSITION:        attrib = bgfx::Attrib::Position;    break;
        case VertexFormat::NORMAL:          attrib = bgfx::Attrib::Normal;      break;
        case VertexFormat::COLOR:           attrib = bgfx::Attrib::Color0;      break;
        case VertexFormat::TANGENT:         attrib = bgfx::Attrib::Tangent;     break;
        case VertexFormat::BINORMAL:        attrib = bgfx::Attrib::Bitangent;   break;
        case VertexFormat::BLENDWEIGHTS:    attrib = bgfx::Attrib::Weight;      break;
        case VertexFormat::BLENDINDICES:    attrib = bgfx::Attrib::Indices;     break;
        case VertexFormat::TEXCOORD0:       attrib = bgfx::Attrib::TexCoord0;   break;
        case VertexFormat::TEXCOORD1:       attrib = bgfx::Attrib::TexCoord1;   break;
        case VertexFormat::TEXCOORD2:       attrib = bgfx::Attrib::TexCoord2;   break;
        case VertexFormat::TEXCOORD3:       attrib = bgfx::Attrib::TexCoord3;   break;
        case VertexFormat::TEXCOORD4:       attrib = bgfx::Attrib::TexCoord4;   break;
        case VertexFormat::TEXCOORD5:       attrib = bgfx::Attrib::TexCoord5;   break;
        case VertexFormat::TEXCOORD6:       attrib = bgfx::Attrib::TexCoord6;   break;
        case VertexFormat::TEXCOORD7:       attrib = bgfx::Attrib::TexCoord7;   break;
        default:                            attrib = bgfx::Attrib::Count;
    }
}

void getBgfxAttributeType(const VertexFormat::Element& element, bgfx::AttribType::Enum &type, bool& normalized)
{
    switch(element.usage)
    {
        case VertexFormat::POSITION:
        case VertexFormat::NORMAL:
        case VertexFormat::TANGENT:
        case VertexFormat::BINORMAL:
        case VertexFormat::BLENDWEIGHTS:
        case VertexFormat::BLENDINDICES:
        case VertexFormat::TEXCOORD0:
        case VertexFormat::TEXCOORD1:
        case VertexFormat::TEXCOORD2:
        case VertexFormat::TEXCOORD3:
        case VertexFormat::TEXCOORD4:
        case VertexFormat::TEXCOORD5:
        case VertexFormat::TEXCOORD6:
        case VertexFormat::TEXCOORD7:
            type = bgfx::AttribType::Float;
            normalized = false;
        break;

        case VertexFormat::COLOR:
            type = bgfx::AttribType::Float;
            normalized = true;
        break;

        default:
            type = bgfx::AttribType::Float;
            normalized = false;
        break;
    }
}

void BGFXVertexBuffer::createVertexDecl(const VertexFormat &vertexFormat, bgfx::VertexDecl& vertexDecl)
{
    vertexDecl.begin();

    for(size_t i=0; i<vertexFormat.getElementCount(); ++i)
    {
        const VertexFormat::Element element = vertexFormat.getElement(i);

        bgfx::Attrib::Enum attrib;
        bgfx::AttribType::Enum type;
        bool normalized;

        getBgfxAttribute(element, attrib);
        getBgfxAttributeType(element, type, normalized);
        uint8_t num = element.size;

        vertexDecl.add(attrib,num,type,normalized,false);
    }

    vertexDecl.end();
}

void BGFXVertexBuffer::set(const void* vertexData, unsigned int vertexCount, unsigned int vertexStart)
{
    /*GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, _vbh) );

    if (vertexStart == 0 && vertexCount == 0)
    {
        GL_ASSERT( glBufferData(GL_ARRAY_BUFFER, _vertexFormat->getVertexSize() * _vertexCount, vertexData, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
    }
    else
    {
        if (vertexCount == 0)
        {
            vertexCount = _vertexCount - vertexStart;
        }

        GL_ASSERT( glBufferSubData(GL_ARRAY_BUFFER, vertexStart * _vertexFormat->getVertexSize(), vertexCount * _vertexFormat->getVertexSize(), vertexData) );
    }*/

    if(vertexCount == 0)
    {
        GP_WARN("BGFXVertexBuffer::set() - vertexCount is null.");
        return;
    }

    uint32_t size = _vertexDecl.getSize(vertexCount);
    const bgfx::Memory* mem = bgfx::copy(vertexData, size);


    if(_dynamic)
    {
        /*if(!bgfx::isValid(_dvbh))
        {
            //uint32_t size = _vertexDecl.getSize(_vertexCount);

            uint16_t flags = BGFX_BUFFER_NONE; //BGFX_BUFFER_ALLOW_RESIZE;
            _dvbh = bgfx::createDynamicVertexBuffer(_vertexCount, _vertexDecl, flags);

            //_vertexData = new char[size];
            //_vertexMem = bgfx::makeRef(_vertexData, size);
            //_vertexData = bgfx::alloc(size);

            //_vertexData = bgfx::copy(vertexData, size);
            //bgfx::updateDynamicVertexBuffer(_dvbh, vertexStart, _vertexData);
        }

        GP_ASSERT(bgfx::isValid(_dvbh));
        //bgfx::updateDynamicVertexBuffer(_dvbh, vertexStart, mem);*/

        // need to resize buffer ?
        unsigned int sizeMax = _memoryBuffer.getSizeMax();
        if(sizeMax < size)
            _memoryBuffer.resize(size);

        // update data
        void * dest = lock(vertexStart, vertexCount, false);
        memcpy(dest, vertexData, size);
        unLock();
    }
    else
    {
        if(!bgfx::isValid(_svbh))
        {
            uint16_t flags = BGFX_BUFFER_NONE;
            _svbh = bgfx::createVertexBuffer(mem, _vertexDecl, flags);
            GP_ASSERT(bgfx::isValid(_svbh));
        }
        else
        {
            GP_WARN("BGFXVertexBuffer::set() - static vertex buffer already set.");
        }
    }
}

void BGFXVertexBuffer::bind()
{
    if(_dynamic)
    {
        if(bgfx::isValid(_dvbh))
            bgfx::setVertexBuffer(0,_dvbh);
        else
            GP_WARN("BGFXVertexBuffer::bind() - dynamic vertex buffer no set.");
    }
    else
    {
        if(bgfx::isValid(_svbh))
            bgfx::setVertexBuffer(0,_svbh);
        else
            GP_WARN("BGFXVertexBuffer::bind() - static vertex buffer no set.");
    }
}




void * BGFXVertexBuffer::lock(unsigned start, unsigned count, bool discard)
{
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
    int debug2 = _vertexDecl.getSize(start+count);
    int debug3 = _memoryBuffer.getSizeMax();
    GP_ASSERT(_memoryBuffer.getSizeMax() >= _vertexDecl.getSize(start+count));


    _lockStart = start;
    _lockCount = count;
    _lockState = LOCK_SCRATCH;
    //_lockScratchData = bx::mem graphics_->ReserveScratchBuffer(count * vertexSize_);
   // _lockScratchData = bgfx::makeRef()
    //const bgfx::Memory* mem = bgfx::alloc()

    _lockScratchData = _memoryBuffer.map(0);

    GP_ASSERT(_lockScratchData);



    return _lockScratchData;

}
void BGFXVertexBuffer::unLock()
{
    GP_ASSERT(bgfx::isValid(_dvbh));

    if (_lockState == LOCK_SCRATCH)
    {
        bgfx::updateDynamicVertexBuffer(_dvbh, _lockStart, bgfx::makeRef(_lockScratchData, _vertexDecl.getSize(_vertexCount)) );

        _memoryBuffer.unmap();

        //setDataRange(shadowData_.Get() + lockStart_ * vertexSize_, lockStart_, lockCount_, discardLock_);
        _lockState = LOCK_NONE;
        _lockScratchData = nullptr;
    }


    //
}

} // end namespace gameplay
