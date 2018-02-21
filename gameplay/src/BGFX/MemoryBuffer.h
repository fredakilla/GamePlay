#pragma once

#include "../Base.h"

namespace gameplay
{

/**
 * The IBuffer interface
 */
class IBuffer
{
public:
    IBuffer();
    virtual ~IBuffer();
    void resize(uint32_t newSize);
    uint32_t getSize();

protected:
    virtual void create(uint32_t newSize);
    virtual void destroy();
    virtual void* map(uint32_t stride = 0);
    virtual void unmap();

    uint32_t _size;
};


/**
 * The MemoryBuffer class
 */
class MemoryBuffer : public IBuffer
{
public:
    MemoryBuffer();
    ~MemoryBuffer();
    void create(uint32_t newSize) override;
    void destroy() override;
    void* map(uint32_t stride = 0) override;
    void unmap() override;

private:
    char* buffer;
};

}
