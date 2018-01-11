#include "R_RenderToTexture.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Renderer", "RenderToTexture", R_RenderToTexture, 2);
#endif


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
        p1.x, p1.y, 0.0f,     1.0f, 0.0f, 0.0f, 1.0f,
        p2.x, p2.y, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f,
        p3.x, p3.y, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,
    };
    unsigned int vertexCount = 3;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 4)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::TRIANGLES);
    mesh->setVertexData(vertices, 0, vertexCount);
    return mesh;
}

static Mesh* createTexturedCube(float size = 1.0f)
{
    float a = size;
    float vertices[] =
    {
         // position     // normal              // texcoord
        -a, -a,  a,      0.0,  0.0,  1.0,       0.0, 0.0,
         a, -a,  a,      0.0,  0.0,  1.0,       1.0, 0.0,
        -a,  a,  a,      0.0,  0.0,  1.0,       0.0, 1.0,
         a,  a,  a,      0.0,  0.0,  1.0,       1.0, 1.0,
        -a,  a,  a,      0.0,  1.0,  0.0,       0.0, 0.0,
         a,  a,  a,      0.0,  1.0,  0.0,       1.0, 0.0,
        -a,  a, -a,      0.0,  1.0,  0.0,       0.0, 1.0,
         a,  a, -a,      0.0,  1.0,  0.0,       1.0, 1.0,
        -a,  a, -a,      0.0,  0.0, -1.0,       0.0, 0.0,
         a,  a, -a,      0.0,  0.0, -1.0,       1.0, 0.0,
        -a, -a, -a,      0.0,  0.0, -1.0,       0.0, 1.0,
         a, -a, -a,      0.0,  0.0, -1.0,       1.0, 1.0,
        -a, -a, -a,      0.0, -1.0,  0.0,       0.0, 0.0,
         a, -a, -a,      0.0, -1.0,  0.0,       1.0, 0.0,
        -a, -a,  a,      0.0, -1.0,  0.0,       0.0, 1.0,
         a, -a,  a,      0.0, -1.0,  0.0,       1.0, 1.0,
         a, -a,  a,      1.0,  0.0,  0.0,       0.0, 0.0,
         a, -a, -a,      1.0,  0.0,  0.0,       1.0, 0.0,
         a,  a,  a,      1.0,  0.0,  0.0,       0.0, 1.0,
         a,  a, -a,      1.0,  0.0,  0.0,       1.0, 1.0,
        -a, -a, -a,     -1.0,  0.0,  0.0,       0.0, 0.0,
        -a, -a,  a,     -1.0,  0.0,  0.0,       1.0, 0.0,
        -a,  a, -a,     -1.0,  0.0,  0.0,       0.0, 1.0,
        -a,  a,  a,     -1.0,  0.0,  0.0,       1.0, 1.0
    };
    short indices[] =
    {
        0, 1, 2,
        2, 1, 3,
        4, 5, 6,
        6, 5, 7,
        8, 9, 10,
        10, 9, 11,
        12, 13, 14,
        14, 13, 15,
        16, 17, 18,
        18, 17, 19,
        20, 21, 22,
        22, 21, 23
    };
    unsigned int vertexCount = 24;
    unsigned int indexCount = 36;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setVertexData(vertices, 0, vertexCount);
    MeshPart* meshPart = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, indexCount, false);
    meshPart->setIndexData(indices, 0, indexCount);
    return mesh;
}


R_RenderToTexture::R_RenderToTexture()
    : _font(NULL)
    , _triangleModel(NULL)
    , _cubeModel(NULL)
    , _spinDirection(-1.0f)
{    
}



void R_RenderToTexture::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");


    // Create an orthographic projection matrix.
    float width = getWidth() / (float)getHeight();
    float height = 1.0f;
    Matrix::createOrthographic(width, height, -1.0f, 1.0f, &_worldViewProjectionMatrix);

    // Create a triangle mesh.
    Mesh* mesh = createTriangleMesh();    
    _triangleModel = Model::create(mesh);
    SAFE_RELEASE(mesh);

    // Create a material for the triangle.
    _triangleModel->setMaterial("res/shaders/colored.vert", "res/shaders/colored_vertex.frag", "VERTEX_COLOR");




    // Create a perspective projection matrix.
    Matrix projMatrix;
    Matrix::createPerspective(45.0f, getWidth() / (float)getHeight(), 1.0f, 100.0f, &projMatrix);

    // Create a lookat view matrix.
    Matrix viewMatrix;
    Matrix::createLookAt(Vector3(3,2,-5), Vector3::zero(), Vector3::unitY(), &viewMatrix);

    // mvp matrix
    Matrix mvp = projMatrix * viewMatrix;


    // Create a frame buffer with render target
    _frameBuffer = FrameBuffer::create("myFrameBuffer", 128, 128);

    Material* material = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag");
    //Texture::Sampler * sampler = Texture::Sampler::create("res/png/brick.png");
    Texture::Sampler * sampler = Texture::Sampler::create(_frameBuffer->getRenderTarget(0)->getTexture());
    material->getParameter("u_diffuseTexture")->setValue(sampler);
    material->getParameter("u_worldViewProjectionMatrix")->setValue(mvp);
    material->getStateBlock()->setCullFace(true);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(true);

    // Create a cube.
    Mesh* meshQuad = createTexturedCube();
    _cubeModel = Model::create(meshQuad);
    _cubeModel->setMaterial(material);
    SAFE_RELEASE(mesh);



    // Set views

    Game * game = Game::getInstance();

    View defaultView;
    defaultView.id = 0;
    defaultView.clearColor = 0x111111ff;
    defaultView.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
    defaultView.depth = 1.0f;
    defaultView.rectangle = Rectangle(game->getWidth(), game->getHeight());
    game->addView(defaultView);

    View secondView;
    secondView.id = 1;
    secondView.clearColor = 0x332266ff;
    secondView.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
    secondView.depth = 1.0f;
    secondView.rectangle = Rectangle(0,0,128,128);
    game->addView(secondView);
}

void R_RenderToTexture::finalize()
{
    SAFE_RELEASE(_cubeModel);
    SAFE_RELEASE(_triangleModel);
    SAFE_RELEASE(_font);
}

void R_RenderToTexture::update(float elapsedTime)
{
    // Update the rotation of the triangle. The speed is 180 degrees per second.
    _worldViewProjectionMatrix.rotateZ( _spinDirection * MATH_PI * elapsedTime * 0.001f);
}

void R_RenderToTexture::render(float elapsedTime)
{
    Rectangle defaultViewport = Game::getInstance()->getViewport();

    // draw the triangle in frame buffer
    Game::getInstance()->bindView(1);
    _frameBuffer->bind();
    _triangleModel->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(_worldViewProjectionMatrix);
    _triangleModel->draw();

    // draw the cube in main view
    Game::getInstance()->bindView(0);
    _cubeModel->draw();

    // draw frame rate
    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void R_RenderToTexture::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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
