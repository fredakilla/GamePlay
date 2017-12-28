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


}
