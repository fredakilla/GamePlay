#ifndef BGFX_RENDERER_H_
#define BGFX_RENDERER_H_

#include "../Base.h"

namespace gameplay
{

class Matrix;
class BGFXGpuProgram;

#define DRAW_DEBUG_TEXT bgfx::dbgTextPrintf


class BGFXRenderer
{
    static BGFXRenderer * _instance;

    struct Caps
    {
        // limits
        uint32_t _maxFrameBufferAttachments;
    };

public:

    static BGFXRenderer & getInstance()
    {
        GP_ASSERT(_instance);
        return *_instance;
    }

    void destroy()
    {
        delete _instance;
    }




    static void initInstance();

    void queryCaps();
    const Caps getCaps() const { return _caps; }

    void updateWindowSize(unsigned int width, unsigned height);
    void toggleDebugStats();

    void toggleWireFrame();

    void toggleVSync();
    void setVSync(bool enable);
    bool isVSync() { return _isVsync; }

    void submit(const BGFXGpuProgram * gpuProgram);

    void beginFrame();
    void endFrame();


    //void setViewTransform(Matrix& view, Matrix& proj, unsigned short viewId = 0);
    //void setModelTransform(Matrix * matrix, unsigned short count = 1);



protected:
    BGFXRenderer();
    ~BGFXRenderer();


    uint32_t _debug_flags;
    uint32_t _reset_flags;


    Caps        _caps;
    unsigned    _width;
    unsigned    _height;
    bool        _isVsync;
};


} // end namespace gameplay

#endif // BGFX_RENDERER_H_
