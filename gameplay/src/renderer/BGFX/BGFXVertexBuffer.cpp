#include "BGFXVertexBuffer.h"

namespace gameplay {


BGFXVertexBuffer::BGFXVertexBuffer(const VertexFormat &vertexFormat, unsigned int vertexCount, bool dynamic)
{
    _vertexFormat = (VertexFormat*)&vertexFormat;
    _vertexCount = vertexCount;
    _dynamic = dynamic;

    _createVertexDecl(vertexFormat);

    _svbh = BGFX_INVALID_HANDLE;
}

BGFXVertexBuffer::~BGFXVertexBuffer()
{
    if(bgfx::isValid(_svbh))
        bgfx::destroy(_svbh);
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

void BGFXVertexBuffer::_createVertexDecl(const VertexFormat &vertexFormat)
{
    _vertexDecl.begin();

    for(size_t i=0; i<vertexFormat.getElementCount(); ++i)
    {
        const VertexFormat::Element element = vertexFormat.getElement(i);

        bgfx::Attrib::Enum attrib;
        bgfx::AttribType::Enum type;
        bool normalized;

        getBgfxAttribute(element, attrib);
        getBgfxAttributeType(element, type, normalized);
        uint8_t num = element.size;

        _vertexDecl.add(attrib,num,type,normalized,false);
    }

    _vertexDecl.end();
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

    if(_dynamic)
    {
        // dynamic vertex buffer

        if(!bgfx::isValid(_dvbh))
        {
            uint32_t size = _vertexDecl.getSize(vertexCount);
            const bgfx::Memory* mem = bgfx::copy(vertexData, size);

            uint16_t flags = BGFX_BUFFER_NONE;
            _dvbh = bgfx::createDynamicVertexBuffer(mem, _vertexDecl, flags);
            GP_ASSERT(bgfx::isValid(_dvbh));

        }
    }
    else
    {
        if(!bgfx::isValid(_svbh))
        {

            uint32_t size = _vertexDecl.getSize(vertexCount);
            //const bgfx::Memory* mem = bgfx::alloc(size);
            const bgfx::Memory* mem = bgfx::copy(vertexData, size);
            //const bgfx::Memory* mem = bgfx::makeRef(membuff, _vertexDecl.getSize(vertexCount));

            uint16_t flags = BGFX_BUFFER_NONE;
            _svbh = bgfx::createVertexBuffer(mem, _vertexDecl, flags);
            GP_ASSERT(bgfx::isValid(_svbh));
        }
    }
}

void BGFXVertexBuffer::bind()
{
    if(_dynamic)
    {
        bgfx::setVertexBuffer(0,_dvbh);

        /*bgfx::setVertexBuffer(0,dp.geometry->vb->dvbh);
        if (dp.geometry->indexed)
            bgfx::setIndexBuffer(dp.geometry->ib->dibh);*/
    }
    else
    {
        bgfx::setVertexBuffer(0,_svbh);
        /*if (dp.geometry->indexed)
            bgfx::setIndexBuffer(dp.geometry->ib->sibh);*/
    }
}

} // end namespace gameplay
