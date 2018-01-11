#ifndef R_DRAWSTRESS_H_
#define R_DRAWSTRESS_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;


class R_DrawStress : public Sample, Control::Listener
{
public:

    R_DrawStress();
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    void initialize();
    void initializeUI();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

    void controlEvent(Control* control, EventType evt);

private:

    void setColoredGeometry();
    void setTexturedGeometry();

    Form* _form;
    Font* _font;
    Model* _model;
    Material* _materialColored;
    Material* _materialTextured;
    Matrix _worldViewProjectionMatrix;
    float _rotValue;
    unsigned int _maxDimensions;
};

#endif
