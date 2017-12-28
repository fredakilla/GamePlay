#ifndef BGFX_RENDERER_H_
#define BGFX_RENDERER_H_

#include "Renderer.h"

namespace gameplay {


class BGFXRenderer : public Renderer
{
public:
    static void initInstance();


    void queryCaps() override;

protected:
    BGFXRenderer();
    ~BGFXRenderer();
};


}

#endif // BGFX_RENDERER_H_
