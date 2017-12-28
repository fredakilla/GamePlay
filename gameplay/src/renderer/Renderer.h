#ifndef RENDERER_H_
#define RENDERER_H_

#include "Base.h"

namespace gameplay
{

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

    //virtual void callMe() = 0;

    void destroy()
    {
        delete _instance;
    }

    virtual void queryCaps() {}
    virtual const Caps getCaps() const { return _caps; }


protected:
    Renderer()
    {
        GP_ASSERT(!_instance); // Instance already exists
        //if (_instance)
        //    throw std::logic_error("Instance already exists");
        _instance = this;
    }

    virtual ~Renderer() { printf("Base Deleted\n"); } // so random strangers can't delete me


    Caps _caps;

};

}

#endif // RENDERER_H_
