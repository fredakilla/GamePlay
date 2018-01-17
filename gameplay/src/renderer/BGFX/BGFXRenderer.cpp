#include "../../renderer/BGFX/BGFXRenderer.h"
#include "../../math/Transform.h"
#include "../../core/Game.h"

#define GP_DRAW_DEBUG   // allow debug text

namespace gameplay {


BGFXRenderer::BGFXRenderer()
{
    printf("BGFXRenderer Created\n");

    _debug_flags = BGFX_DEBUG_TEXT;
    _reset_flags = BGFX_RESET_NONE;
}

BGFXRenderer::~BGFXRenderer()
{
    printf("BGFXRenderer Deleted\n");
}

void BGFXRenderer::initInstance()
{
    new BGFXRenderer;
}

void BGFXRenderer::updateWindowSize(unsigned int width, unsigned height)
{
    _width = width;
    _height = height;

    bgfx::reset(_width, _height, _reset_flags);
}

void BGFXRenderer::toggleDebugStats()
{
    if ((_debug_flags & BGFX_DEBUG_STATS) == 0)
        _debug_flags |= BGFX_DEBUG_STATS;
    else
        _debug_flags &= ~BGFX_DEBUG_STATS;

    bgfx::setDebug(_debug_flags);
}

void BGFXRenderer::toggleVSync()
{
    if (_isVsync)
        setVSync(false);
    else
        setVSync(true);
}

void BGFXRenderer::setVSync(bool enable)
{
    _isVsync = enable;

    if (enable)
        _reset_flags |= BGFX_RESET_VSYNC;
    else
        _reset_flags &= ~BGFX_RESET_VSYNC;

    bgfx::reset(_width, _height, _reset_flags);
}

void BGFXRenderer::queryCaps()
{
    // Query caps and limits.
    _caps._maxFrameBufferAttachments = bgfx::getCaps()->limits.maxFBAttachments;
}

void BGFXRenderer::beginFrame()
{
#ifdef GP_DRAW_DEBUG
    bgfx::dbgTextClear();
#endif

    bgfx::touch(0);
}

void BGFXRenderer::endFrame()
{
    bgfx::frame();
}

void BGFXRenderer::submit(const GpuProgram * gpuProgram)
{
    const BGFXGpuProgram * bgfxGpuProgram = static_cast<const BGFXGpuProgram*>(gpuProgram);
    GP_ASSERT(bgfx::isValid(bgfxGpuProgram->getProgram()));
    bgfx::submit(Game::__curentViewId, bgfxGpuProgram->getProgram());
}

} // end namespace gameplay
