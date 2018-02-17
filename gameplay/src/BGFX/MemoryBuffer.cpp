#include "../BGFX/MemoryBuffer.h"

namespace gameplay
{

IBuffer::IBuffer()
{
    _size = 0;
}

IBuffer::~IBuffer()
{
    destroy();
}

void IBuffer::create(int newSize)
{
    destroy();
    _size = newSize;
}

void IBuffer::destroy()
{
    _size = 0;
}

void IBuffer::resize(int newSize)
{
    if (_size < newSize)
        create(newSize);
    _size = newSize;
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


}
