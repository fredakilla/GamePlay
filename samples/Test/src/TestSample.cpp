#include "TestSample.h"

// Declare our game instance
TestSample game;


/**
 * Creates a triangle mesh with vertex colors.
 */
static Mesh* createTriangleMesh()
{
    // Calculate the vertices of the equilateral triangle.
    float a = 0.5f;     // length of the side
    Vector2 p1(0.0f,       a / sqrtf(3.0f));
    Vector2 p2(-a / 2.0f, -a / (2.0f * sqrtf(3.0f)));
    Vector2 p3( a / 2.0f, -a / (2.0f * sqrtf(3.0f)));

    // Create 3 vertices. Each vertex has position (x, y, z) and color (red, green, blue)
    float vertices[] =
    {
        p1.x, p1.y, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,        0.0f, 0.0f,
        p2.x, p2.y, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,        1.0f, 0.0f,
        p3.x, p3.y, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,        0.0f, 1.0f,
    };
    unsigned int vertexCount = 3;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 4),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::TRIANGLES);
    mesh->setVertexData(vertices, 0, vertexCount);
    return mesh;
}


TestSample::TestSample()
    : _model(NULL), _spinDirection(-1.0f)
{
}

void TestSample::initialize()
{
    // Create an orthographic projection matrix.
    float width = getWidth() / (float)getHeight();
    float height = 1.0f;
    Matrix::createOrthographic(width, height, -1.0f, 1.0f, &_worldViewProjectionMatrix);

    // Create the triangle mesh.
    Mesh* mesh = createTriangleMesh();

    // Create a model for the triangle mesh. A model is an instance of a Mesh that can be drawn with a specified material.
    _model = Model::create(mesh);
    SAFE_RELEASE(mesh);

    // Create a material from the built-in "colored-unlit" vertex and fragment shaders.
    // This sample doesn't use lighting so the unlit shader is used.
    // This sample uses vertex color so VERTEX_COLOR is defined. Look at the shader source files to see the supported defines.
    //Material * material = _model->setMaterial("res/bgfxshaders/Colored_VS.bin", "res/bgfxshaders/Colored_FS.bin", "VERTEX_COLOR");
    Material * material = _model->setMaterial("res/bgfxshaders/Textured_VS.bin", "res/bgfxshaders/Textured_FS.bin", "VERTEX_COLOR");



    // Load the texture from file.
    bool mipmap = true;
    Texture::Sampler* sampler = material->getParameter("u_diffuseTexture")->setValue("res/png/color-wheel.png", mipmap);
    if (mipmap)
        sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);
    else
        sampler->setFilterMode(Texture::LINEAR, Texture::LINEAR);
    sampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);



    // create material parameter
    material->getParameter("u_diffuseColor")->setValue(Vector4(1.0,1.0,1.0,1.0));

    // create material animation
    unsigned int keyCount = 3;
    unsigned int keyTimes[] = {0, 1000, 2000};
    float keyValues[] =
    {
        0.0f, 0.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0f, 0.0,
        0.0f, 0.0f, 0.0f, 0.0
    };
    MaterialParameter * param = material->getParameter("u_diffuseColor");
    Animation* sampleAnim = param->createAnimation("sample", MaterialParameter::ANIMATE_UNIFORM, keyCount, keyTimes, keyValues, Curve::LINEAR);
    AnimationClip* animClip = sampleAnim->getClip();
    animClip->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);
    animClip->play();
}

void TestSample::finalize()
{
    // Model and font are reference counted and should be released before closing this sample.
    SAFE_RELEASE(_model);
}

void TestSample::update(float elapsedTime)
{
    // Update the rotation of the triangle. The speed is 180 degrees per second.
    _worldViewProjectionMatrix.rotateZ( _spinDirection * MATH_PI * elapsedTime * 0.001f);
}

void TestSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Bind the view projection matrix to the model's parameter. This will transform the vertices when the model is drawn.
    _model->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(_worldViewProjectionMatrix);
    _model->draw();
}

void TestSample::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        }
    }
}

void TestSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        _spinDirection *= -1.0f;
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}
