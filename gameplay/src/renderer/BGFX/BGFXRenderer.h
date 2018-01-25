#ifndef BGFX_RENDERER_H_
#define BGFX_RENDERER_H_

#include "../../renderer/Renderer.h"
#include "../../renderer/BGFX/BGFXGpuProgram.h"

namespace gameplay
{

class BGFXRenderer : public Renderer
{
public:
    static void initInstance();

    void queryCaps() override;
    void updateWindowSize(unsigned int width, unsigned height) override;
    void toggleDebugStats() override;
    void toggleVSync() override;
    void setVSync(bool enable) override;

    void submit(const GpuProgram * gpuProgram) override;

    void beginFrame();
    void endFrame();


    void setViewTransform(Matrix& view, Matrix& proj, unsigned short viewId = 0);
    void setModelTransform(Matrix * matrix, unsigned short count = 1);



protected:
    BGFXRenderer();
    ~BGFXRenderer();

    uint32_t _debug_flags;
    uint32_t _reset_flags;
};


} // end namespace gameplay

#endif // BGFX_RENDERER_H_
