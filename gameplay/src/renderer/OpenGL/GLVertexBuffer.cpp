#include "GL2VertexBuffer.h"

namespace gameplay {


GL2VertexBuffer::GL2VertexBuffer(const VertexFormat &vertexFormat, unsigned int vertexCount, bool dynamic)
{
    _vertexFormat = (VertexFormat*)&vertexFormat;
    _vertexCount = vertexCount;
    _dynamic = dynamic;

    GL_ASSERT( glGenBuffers(1, &_vbh) );
    GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, _vbh) );
    GL_ASSERT( glBufferData(GL_ARRAY_BUFFER, vertexFormat.getVertexSize() * vertexCount, NULL, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
}

GL2VertexBuffer::~GL2VertexBuffer()
{
    glDeleteBuffers(1, &_vbh);
}


void GL2VertexBuffer::set(const void* vertexData, unsigned int vertexCount, unsigned int vertexStart)
{
    GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, _vbh) );

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
    }
}

} // end namespace gameplay
