#ifndef RENDERER_H_
#define RENDERER_H_

#include "../core/Base.h"

namespace gameplay
{

#define DRAW_DEBUG_TEXT bgfx::dbgTextPrintf



class GpuProgram;

class Renderer
{
    static Renderer * _instance;

    struct Caps
    {
        // limits
        uint32_t _maxFrameBufferAttachments;
    };



public:
    static Renderer & getInstance()
    {
        GP_ASSERT(_instance);
        return *_instance;
    }

    void destroy()
    {
        delete _instance;
    }

    virtual void queryCaps() {}
    virtual const Caps getCaps() const { return _caps; }
    virtual void toggleDebugStats() {}
    virtual void updateWindowSize(unsigned int width, unsigned height) {}
    virtual void toggleVSync() {}
    virtual void setVSync(bool enable) {}
    bool isVSync() { return _isVsync; }
    virtual void beginFrame() {}
    virtual void endFrame() {}
    virtual void submit(const GpuProgram* gpuProgram) = 0;


protected:
    Renderer()
    {
        GP_ASSERT(!_instance); // Instance already exists        
        _instance = this;
    }

    virtual ~Renderer() { print("Renderer Deleted\n"); }


    Caps        _caps;
    unsigned    _width;
    unsigned    _height;
    bool        _isVsync;

};

}

#endif // RENDERER_H_
