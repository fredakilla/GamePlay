#ifndef R_LIGHTS_H_
#define R_LIGHTS_H_

#include "gameplay.h"
#include "Sample.h"
#include "FirstPersonCamera.h"

using namespace gameplay;


class R_Sponza : public Sample
{
public:

    R_Sponza();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void keyEvent(Keyboard::KeyEvent evt, int key);
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);


protected:


    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

    void updateCamera(float elapsedTime);


private:

    bool drawScene(Node* node);

private:

    Scene* _scene;

    Font* _font;
    float _spinDirection;
    Matrix _worldViewProjectionMatrix;
    Model* _myModel;


    FirstPersonCamera _fpCamera;
    unsigned int _moveFlags;
    int _prevX;
    int _prevY;
    bool _buttonPressed;
};

#endif
