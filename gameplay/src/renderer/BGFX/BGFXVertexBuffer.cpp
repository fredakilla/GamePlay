#include "BGFXVertexBuffer.h"

namespace gameplay {


BGFXVertexBuffer::BGFXVertexBuffer(const VertexFormat &vertexFormat, unsigned int vertexCount, bool dynamic)
{
    _vertexFormat = (VertexFormat*)&vertexFormat;
    _vertexCount = vertexCount;
    _dynamic = dynamic;

    createVertexDecl(vertexFormat, _vertexDecl);

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
        if(!bgfx::isValid(_dvbh))
        {
            uint16_t flags = BGFX_BUFFER_ALLOW_RESIZE;
            _dvbh = bgfx::createDynamicVertexBuffer(vertexCount, _vertexDecl, flags);
        }

        GP_ASSERT(bgfx::isValid(_dvbh));
        bgfx::updateDynamicVertexBuffer(_dvbh, vertexStart, mem);
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

} // end namespace gameplay
