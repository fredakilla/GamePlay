#pragma once

#include "../BGFX/GeometryBuffer.h"
#include "../Base.h"
#include "../Mesh.h"

namespace gameplay
{

class BGFXIndexBuffer : public GeometryBuffer
{
public:
    BGFXIndexBuffer(const Mesh::IndexFormat indexFormat, uint32_t indexCount, bool dynamic);
    virtual ~BGFXIndexBuffer();
    void set(const void* data, uint32_t count, uint32_t start) override;
    void bind() override;
    void* lock(uint32_t start, uint32_t count) override;
    void unLock() override;

private:
    void createStaticBuffer();
    void createDynamicBuffer();

    bgfx::IndexBufferHandle _sibh;              // static index buffer handle
    bgfx::DynamicIndexBufferHandle _dibh;       // dynamic index buffer handle
    Mesh::IndexFormat _indexFormat;             // 8, 16 or 32 bits
};

} // end namespace gameplay

