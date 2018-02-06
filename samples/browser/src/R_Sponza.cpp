#include "R_Sponza.h"
#include "SamplesGame.h"
#include "renderer/Renderer.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Renderer", "Sponza", R_Sponza, 6);
#endif


    static const unsigned int MOVE_FORWARD = 1;
    static const unsigned int MOVE_BACKWARD = 2;
    static const unsigned int MOVE_LEFT = 4;
    static const unsigned int MOVE_RIGHT = 8;
    static const unsigned int MOVE_UP = 16;
    static const unsigned int MOVE_DOWN = 32;
    static const float MOVE_SPEED = 15.0f;
    static const float UP_DOWN_SPEED = 10.0f;



R_Sponza::R_Sponza()
    : _font(NULL)
    , _myModel(NULL)
    , _spinDirection(-1.0f)
    , _moveFlags(0), _prevX(0), _prevY(0), _buttonPressed(false)
{    
}



void R_Sponza::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");



    //_scene = Scene::load("res/common/sample.scene");




   /* _scene = Scene::load("res/common/duck.gpb");
    Node * _modelNode = _scene->findNode("duck");
    Model* model = dynamic_cast<Model*>(_modelNode->getDrawable());
    Material* material = model->setMaterial("res/common/duck.material");*/



   _scene = Scene::load("res/sponza/sponza.gpb");


   /* Node * node = _scene->findNode("mesh_19");
    _myModel = dynamic_cast<Model*>(node->getDrawable());
    _myModel->setMaterial("res/sponza/sponza.material");*/
    //node->setScale(0.1f);






    // Create a perspective projection matrix.
    Matrix projMatrix;
    Matrix::createPerspective(60.0f, getWidth() / (float)getHeight(), 0.1f, 100000.0f, &projMatrix);
    // Create a lookat view matrix.
    Matrix viewMatrix;
    Matrix::createLookAt(Vector3(0,2,-5), Vector3::zero(), Vector3::unitY(), &viewMatrix);
    // set view transform
    Renderer::getInstance().setViewTransform(viewMatrix, projMatrix);




/*

    // create  material
    Material* material = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag");

    Texture::Sampler * sampler1 = Texture::Sampler::create("res/png/brick.png");
    material->getParameter("s_texColor")->setValue(sampler1);

    Texture::Sampler * sampler2 = Texture::Sampler::create("res/png/brickn.png");
    material->getParameter("s_texNormal")->setValue(sampler2);

    //material->getParameter("u_worldViewProjectionMatrix")->setValue(_worldViewProjectionMatrix);


    material->getStateBlock()->setCullFace(true);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(true);


    Matrix modelMatrix;
    modelMatrix.setIdentity();
    modelMatrix.translate(0,0,0);
    modelMatrix.scale(1.0f);
    modelMatrix.rotate(Vector3(0,1,0),  MATH_DEG_TO_RAD(35));


    _myModel->setMaterial(material);
    //_myModel->getNode()->set(Vector3(0.01,0.01,0.01), Quaternion(Vector3(0,1,0), MATH_DEG_TO_RAD(-45.0f)), Vector3(0,0,0));
    //_myModel->getModelMatrix().set(Transform(Vector3(0.01,0.01,0.01), Quaternion(Vector3(0,1,0), MATH_DEG_TO_RAD(45.0f)), Vector3(0,0,0)).getMatrix());
    _myModel->getModelMatrix().set(modelMatrix);


*/



    // set camera

    Vector3 cameraPosition(0, 1, 10);
    _fpCamera.initialize(1.0, 100000.0f);
    _fpCamera.setPosition(cameraPosition);
    _scene->addNode(_fpCamera.getRootNode());
    _scene->setActiveCamera(_fpCamera.getCamera());

    // Update the aspect ratio for our scene's camera to match the current device resolution
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());














    // Set views
    Game * game = Game::getInstance();
    View defaultView;
    defaultView.id = 0;
    defaultView.clearColor = 0x111111ff;
    defaultView.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
    defaultView.depth = 1.0f;
    defaultView.rectangle = Rectangle(game->getWidth(), game->getHeight());
    game->addView(defaultView);
}

void R_Sponza::finalize()
{
    SAFE_RELEASE(_myModel);
    SAFE_RELEASE(_font);
}

void R_Sponza::update(float elapsedTime)
{
    updateCamera(elapsedTime);



    // Update the rotation of the cube.
    float dt = elapsedTime * 0.001f;
    float dx = 0;//_spinDirection * MATH_PI;
    float dy = 0;//_spinDirection * MATH_PI;
    float dz = _spinDirection * MATH_PI;
    Quaternion rot = Quaternion(Vector3(dx, dy, dz), dt * 2);
    //_myModel->getModelMatrix().rotate(rot);
    //_myModel->getNode()->rotate(rot);


    /*Matrix m;
    m.rotateY(_spinDirection * MATH_PI * elapsedTime * 0.001f);
    //_cubeModel->setModelTransform(m);*/
    ///_myModel->getModelMatrix().rotateZ(_spinDirection * MATH_PI * elapsedTime * 0.001f );
    //_cubeModel->getNode()->rotateX(_spinDirection * MATH_PI * elapsedTime * 0.001f * 1.1);
    //_cubeModel->getNode()->rotateY(_spinDirection * MATH_PI * elapsedTime * 0.001f * 1.4);
    ///_myModel->getNode()->rotateZ(_spinDirection * MATH_PI * elapsedTime * 0.001f * 0.8);

}

void R_Sponza::render(float elapsedTime)
{
    // draw the cube in main view
    Game::getInstance()->bindView(0);


    int m_numLights = 4;
    float time = Game::getAbsoluteTime() * 0.001f;

    Vector4 lightPosRadius[4];
    for (uint32_t ii = 0; ii < m_numLights; ++ii)
    {
        lightPosRadius[ii].x = sin( (time*(0.1f + ii*0.17f) + ii*MATH_PIOVER4*1.37f ) )*3.0f;
        lightPosRadius[ii].y = cos( (time*(0.2f + ii*0.29f) + ii*MATH_PIOVER4*1.49f ) )*3.0f;
        lightPosRadius[ii].z = -2.5f;
        lightPosRadius[ii].w = 3.0f;
    }

    Vector4 lightRgbInnerR[4] =
    {
        { 1.0f, 0.7f, 0.2f, 0.8f },
        { 0.7f, 0.2f, 1.0f, 0.8f },
        { 0.2f, 1.0f, 0.7f, 0.8f },
        { 1.0f, 0.4f, 0.2f, 0.8f },
    };

  /*-cubeModel->getMaterial()->getParameter("u_lightPosRadius")->setValue(lightPosRadius, m_numLights);
  _cubeModel->getMaterial()->getParameter("u_lightRgbInnerR")->setValue(lightRgbInnerR, m_numLights);
*/



   /*float mtx[16];
        bx::mtxIdentity(mtx);
        bx::mtxSRT(mtx, 0.01, 0.01, 0.01,  0,0.25,0,  -2,0,-1);
        //bx::mtxScale(mtx, 0.01f);
        bgfx::setTransform(mtx);*/

       /* Matrix modelMatrix;
        modelMatrix.setIdentity();
        modelMatrix.translate(0,0,0);
        modelMatrix.rotate(Vector3(0,1,0),  MATH_DEG_TO_RAD(35));
        modelMatrix.scale(0.01);
    Renderer::getInstance().setModelTransform(&modelMatrix);*/

   // _cubeModel->getNode()->set(Vector3(0.01,0.01,0.01), Quaternion(Vector3(0,1,0), MATH_DEG_TO_RAD(45.0f)), Vector3(0,0,0));

   // _myModel->draw();



     _scene->visit(this, &R_Sponza::drawScene);


    // draw frame rate
    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}


bool R_Sponza::drawScene(Node* node)
{
    Drawable* drawable = node->getDrawable();
    if (drawable)
        drawable->draw();
    return true;
}


void R_Sponza::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
    {
        if (x < 75 && y < 50)
        {
            // Toggle Vsync if the user touches the top left corner
            setVsync(!isVsync());
        }
        _prevX = x;
        _prevY = y;
        break;
    }
    case Touch::TOUCH_RELEASE:
    {
        _prevX = 0;
        _prevY = 0;
        break;
     }
    case Touch::TOUCH_MOVE:
    {
        int deltaX = x - _prevX;
        int deltaY = y - _prevY;
        _prevX = x;
        _prevY = y;
        float pitch = -MATH_DEG_TO_RAD(deltaY * 0.5f);
        float yaw = MATH_DEG_TO_RAD(deltaX * 0.5f);
        _fpCamera.rotate(yaw, pitch);
        break;
    }
    };
}



void R_Sponza::updateCamera(float elapsedTime)
{
    float time = (float)elapsedTime / 1000.0f;

    Vector2 move;

    if (_moveFlags != 0)
    {
        // Forward motion
        if (_moveFlags & MOVE_FORWARD)
        {
            move.y = 1;
        }
        else if (_moveFlags & MOVE_BACKWARD)
        {
            move.y = -1;
        }
        // Strafing
        if (_moveFlags & MOVE_LEFT)
        {
            move.x = 1;
        }
        else if (_moveFlags & MOVE_RIGHT)
        {
            move.x = -1;
        }
        move.normalize();

        // Up and down
        if (_moveFlags & MOVE_UP)
        {
            _fpCamera.moveUp(time * UP_DOWN_SPEED);
        }
        else if (_moveFlags & MOVE_DOWN)
        {
            _fpCamera.moveDown(time * UP_DOWN_SPEED);
        }
    }
    /*else if (_gamepad->getJoystickCount() > 0)
    {
        _gamepad->getJoystickValues(0, &move);
        move.x = -move.x;
    }
    if (_gamepad->getJoystickCount() > 1)
    {
        Vector2 joy2;
        _gamepad->getJoystickValues(1, &joy2);
        _fpCamera.rotate(MATH_DEG_TO_RAD(joy2.x * 2.0f), MATH_DEG_TO_RAD(joy2.y * 2.0f));
    }*/

    if (!move.isZero())
    {
        move.scale(time * MOVE_SPEED);
        _fpCamera.moveForward(move.y);
        _fpCamera.moveLeft(move.x);
    }

    /*if (!_buttonPressed && _gamepad->isButtonDown(Gamepad::BUTTON_A))
    {
        addSound("footsteps.wav");
    }
    _buttonPressed = _gamepad->isButtonDown(Gamepad::BUTTON_A);*/
}



void R_Sponza::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
            _moveFlags |= MOVE_FORWARD;
            break;
        case Keyboard::KEY_S:
            _moveFlags |= MOVE_BACKWARD;
            break;
        case Keyboard::KEY_A:
            _moveFlags |= MOVE_LEFT;
            break;
        case Keyboard::KEY_D:
            _moveFlags |= MOVE_RIGHT;
            break;

        case Keyboard::KEY_Q:
            _moveFlags |= MOVE_DOWN;
            break;
        case Keyboard::KEY_E:
            _moveFlags |= MOVE_UP;
            break;
        case Keyboard::KEY_PG_UP:
            _fpCamera.rotate(0, MATH_PIOVER4);
            break;
        case Keyboard::KEY_PG_DOWN:
            _fpCamera.rotate(0, -MATH_PIOVER4);
            break;
        }
    }
    else if (evt == Keyboard::KEY_RELEASE)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
            _moveFlags &= ~MOVE_FORWARD;
            break;
        case Keyboard::KEY_S:
            _moveFlags &= ~MOVE_BACKWARD;
            break;
        case Keyboard::KEY_A:
            _moveFlags &= ~MOVE_LEFT;
            break;
        case Keyboard::KEY_D:
            _moveFlags &= ~MOVE_RIGHT;
            break;
        case Keyboard::KEY_Q:
            _moveFlags &= ~MOVE_DOWN;
            break;
        case Keyboard::KEY_E:
            _moveFlags &= ~MOVE_UP;
            break;
        }
    }
}

bool R_Sponza::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch (evt)
    {
    case Mouse::MOUSE_WHEEL:
        _fpCamera.moveForward(wheelDelta * MOVE_SPEED / 2.0f );
        return true;
    }
    return false;
}


