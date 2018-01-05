#include "BGFXRenderer.h"

#include <bx/math.h>
#include "Transform.h"
#include "Game.h"

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







    /*float at[3]  = { 0.0f, 0.0f,   0.0f };
    float eye[3] = { 0.0f, 0.0f, -10.0f };

    float view[16];
    bx::mtxLookAt(view, eye, at);

    float proj[16];
    bx::mtxProj(proj, 60.0f, float(1280)/float(720), 0.1f, 10000.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(0, view, proj);*/

   /*float ortho[16];
    bx::mtxOrtho(ortho, 0.0f, 1280.0f, 720.0f, 0.0f, 0.0f, 1000.0f, 0.0, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(0, NULL, ortho);*/




    // Set view 0 default viewport.
    //bgfx::setViewRect(0, 0, 0, uint16_t(1280), uint16_t(720) );


    /*Transform t;
    t.set(Vector3(1,1,1), Quaternion::identity(), Vector3::zero());
    bgfx::setTransform(t.getMatrix().m);*/

    /*float mtx[16];
    bx::mtxIdentity(mtx);
    bgfx::setTransform(mtx);*/


    // Set render states.
    /*bgfx::setState(0
                   //| BGFX_STATE_DEFAULT

                   | BGFX_STATE_RGB_WRITE
                   | BGFX_STATE_ALPHA_WRITE
                   | BGFX_STATE_DEPTH_TEST_LESS
                   | BGFX_STATE_DEPTH_WRITE
                   //| BGFX_STATE_CULL_CW
                   | BGFX_STATE_MSAA


                   | BGFX_STATE_PT_TRISTRIP
                   | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
                   );*/


    bgfx::submit(Game::__curentViewId, bgfxGpuProgram->getProgram());
}

}
