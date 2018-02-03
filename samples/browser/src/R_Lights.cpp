#include "R_Lights.h"
#include "SamplesGame.h"
#include "renderer/Renderer.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Renderer", "Lights", R_Lights, 5);
#endif



R_Lights::R_Lights()
    : _font(NULL)
    , _myModel(NULL)
    , _spinDirection(-1.0f)
{    
}



void R_Lights::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");





    Scene * scene = Scene::load("res/common/duck.gpb");
    Node * node = scene->findNode("duck");
    _myModel = dynamic_cast<Model*>(node->getDrawable());
    //_myModel->setMaterial("res/common/duck.material");






    // Create a perspective projection matrix.
    Matrix projMatrix;
    Matrix::createPerspective(60.0f, getWidth() / (float)getHeight(), 0.1f, 1000.0f, &projMatrix);
    // Create a lookat view matrix.
    Matrix viewMatrix;
    Matrix::createLookAt(Vector3(0,2,-5), Vector3::zero(), Vector3::unitY(), &viewMatrix);
    // set view transform
    Renderer::getInstance().setViewTransform(viewMatrix, projMatrix);






    // create  material
    Material* material = Material::create("res/shaders/debug2.vert", "res/shaders/debug2.frag");

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
    modelMatrix.scale(0.01);
    modelMatrix.rotate(Vector3(0,1,0),  MATH_DEG_TO_RAD(35));


    _myModel->setMaterial(material);
    //_myModel->getNode()->set(Vector3(0.01,0.01,0.01), Quaternion(Vector3(0,1,0), MATH_DEG_TO_RAD(-45.0f)), Vector3(0,0,0));
    //_myModel->getModelMatrix().set(Transform(Vector3(0.01,0.01,0.01), Quaternion(Vector3(0,1,0), MATH_DEG_TO_RAD(45.0f)), Vector3(0,0,0)).getMatrix());
    _myModel->getModelMatrix().set(modelMatrix);



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

void R_Lights::finalize()
{
    SAFE_RELEASE(_myModel);
    SAFE_RELEASE(_font);
}

void R_Lights::update(float elapsedTime)
{
    // Update the rotation of the cube.
    float dt = elapsedTime * 0.001f;
    float dx = 0;//_spinDirection * MATH_PI;
    float dy = 0;//_spinDirection * MATH_PI;
    float dz = _spinDirection * MATH_PI;
    Quaternion rot = Quaternion(Vector3(dx, dy, dz), dt * 2);
    _myModel->getModelMatrix().rotate(rot);
    //_myModel->getNode()->rotate(rot);


    /*Matrix m;
    m.rotateY(_spinDirection * MATH_PI * elapsedTime * 0.001f);
    //_cubeModel->setModelTransform(m);*/
    ///_myModel->getModelMatrix().rotateZ(_spinDirection * MATH_PI * elapsedTime * 0.001f );
    //_cubeModel->getNode()->rotateX(_spinDirection * MATH_PI * elapsedTime * 0.001f * 1.1);
    //_cubeModel->getNode()->rotateY(_spinDirection * MATH_PI * elapsedTime * 0.001f * 1.4);
    ///_myModel->getNode()->rotateZ(_spinDirection * MATH_PI * elapsedTime * 0.001f * 0.8);

}

void R_Lights::render(float elapsedTime)
{
    // draw the cube in main view
    Game::getInstance()->bindView(0);
    _myModel->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(_worldViewProjectionMatrix);




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

    _myModel->draw();

    // draw frame rate
    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void R_Lights::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        // Reverse the spin direction if the user touches the screen.
        _spinDirection *= -1.0f;
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}
