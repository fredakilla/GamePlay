#include "BGFXRenderer.h"

namespace gameplay {


BGFXRenderer::BGFXRenderer()
{
    printf("Derived Created\n");
}

BGFXRenderer::~BGFXRenderer()
{
    printf("Derived Deleted\n");
}

void BGFXRenderer::initInstance()
{
    new BGFXRenderer;
}


void BGFXRenderer::queryCaps()
{
    // Query caps and limits.
    _caps._maxFrameBufferAttachments = bgfx::getCaps()->limits.maxFBAttachments;
}


void BGFXRenderer::submit(const GpuProgram * gpuProgram)
{
    const BGFXGpuProgram * bgfxGpuProgram = static_cast<const BGFXGpuProgram*>(gpuProgram);

    GP_ASSERT(bgfx::isValid(bgfxGpuProgram->getProgram()));

    bgfx::submit(0, bgfxGpuProgram->getProgram());
}

}
