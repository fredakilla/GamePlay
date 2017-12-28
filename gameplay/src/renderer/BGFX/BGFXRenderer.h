#ifndef BGFX_RENDERER_H_
#define BGFX_RENDERER_H_

#include "Renderer.h"
#include "BGFXGpuProgram.h"

namespace gameplay {


class BGFXRenderer : public Renderer
{
public:
    static void initInstance();


    void queryCaps() override;

    void submit(const GpuProgram * gpuProgram) override;

protected:
    BGFXRenderer();
    ~BGFXRenderer();
};


}

#endif // BGFX_RENDERER_H_
