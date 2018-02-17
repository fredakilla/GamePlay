#ifndef MEMORYBUFFER_H_
#define MEMORYBUFFER_H_

namespace gameplay
{

/// Vertex/index buffer lock state.
enum LockState
{
    LOCK_NONE = 0,
    LOCK_ACTIVE,
};

/// Base Buffer
class IBuffer
{
public:
    IBuffer();
    virtual ~IBuffer();
    void resize(int newSize);
    int getSize() { return _size; }

protected:
    virtual void create(int newSize);
    virtual void destroy();
    virtual void* map(int stride) { return nullptr; }
    virtual void unmap() {}

protected:
    int _size;
};


/// Memory buffer
class MemoryBuffer : public IBuffer
{
public:
    MemoryBuffer();
    ~MemoryBuffer();
    void create(int newSize);
    void destroy();
    void* map(int stride) { return buffer; }
    void unmap() {}

private:
    char* buffer;
};

}

#endif
