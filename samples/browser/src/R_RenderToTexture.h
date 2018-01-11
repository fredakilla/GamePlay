#ifndef R_RENDERTOTEXTURE_H_
#define R_RENDERTOTEXTURE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;


class R_RenderToTexture : public Sample
{
public:

    R_RenderToTexture();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

private:

    Font* _font;
    Model* _triangleModel;
    float _spinDirection;
    Matrix _worldViewProjectionMatrix;
    Model* _cubeModel;
    FrameBuffer * _frameBuffer;
};

#endif
