#ifndef R_LIGHTS_H_
#define R_LIGHTS_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;


class R_Lights : public Sample
{
public:

    R_Lights();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

private:

    Font* _font;
    float _spinDirection;
    Matrix _worldViewProjectionMatrix;
    Model* _myModel;
};

#endif
