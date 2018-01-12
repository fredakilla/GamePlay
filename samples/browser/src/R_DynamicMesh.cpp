#include "R_DynamicMesh.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Renderer", "DyanmicMeshUpdate", R_DynamicMeshUpdate, 3);
#endif



    class IBuffer
    {
    public:
        IBuffer();
        virtual ~IBuffer();

        void setSize(int newSize)
        {
            if (_sizeMax < newSize)
                create(newSize);
            _size = newSize;
        }
        virtual void create(int newSize);
        virtual void destroy();
        virtual void* lock(int stride) { return nullptr; }
        virtual void unLock() {}

    protected:
        int _sizeMax;
        int _size;
    };

    class MemoryBuffer : public IBuffer
    {
    public:

        MemoryBuffer();
        virtual ~MemoryBuffer();
        virtual void create(int newSize);
        virtual void destroy();
        virtual void* lock(int stride) { return buffer; }

    private:
        char* buffer;
    };


    IBuffer::IBuffer()
    {
        _sizeMax = 0;
        _size = 0;
    }

    IBuffer::~IBuffer()
    {
        destroy();
    }

    void IBuffer::create(int newSize)
    {
        destroy();
        _sizeMax = newSize;
    }

    void IBuffer::destroy()
    {
        _sizeMax = 0;
        _size = 0;
    }

    // --------------------------------

    MemoryBuffer::MemoryBuffer() : IBuffer()
    {
        buffer = nullptr;
    }

    MemoryBuffer::~MemoryBuffer()
    {
        destroy();
    }

    void MemoryBuffer::create(int newSize)
    {
        IBuffer::create(newSize);
        buffer = new char[newSize];
    }

    void MemoryBuffer::destroy()
    {
        IBuffer::destroy();

        if (buffer)
        {
            delete[] buffer;
            buffer = nullptr;
        }
    }


















    float vertices[] =
    {
         // position     // normal              // texcoord
        -1, -1,  1,      0.0,  0.0,  1.0,       0.0, 0.0,
         1, -1,  1,      0.0,  0.0,  1.0,       1.0, 0.0,
        -1,  1,  1,      0.0,  0.0,  1.0,       0.0, 1.0,
         1,  1,  1,      0.0,  0.0,  1.0,       1.0, 1.0,
        -1,  1,  1,      0.0,  1.0,  0.0,       0.0, 0.0,
         1,  1,  1,      0.0,  1.0,  0.0,       1.0, 0.0,
        -1,  1, -1,      0.0,  1.0,  0.0,       0.0, 1.0,
         1,  1, -1,      0.0,  1.0,  0.0,       1.0, 1.0,
        -1,  1, -1,      0.0,  0.0, -1.0,       0.0, 0.0,
         1,  1, -1,      0.0,  0.0, -1.0,       1.0, 0.0,
        -1, -1, -1,      0.0,  0.0, -1.0,       0.0, 1.0,
         1, -1, -1,      0.0,  0.0, -1.0,       1.0, 1.0,
        -1, -1, -1,      0.0, -1.0,  0.0,       0.0, 0.0,
         1, -1, -1,      0.0, -1.0,  0.0,       1.0, 0.0,
        -1, -1,  1,      0.0, -1.0,  0.0,       0.0, 1.0,
         1, -1,  1,      0.0, -1.0,  0.0,       1.0, 1.0,
         1, -1,  1,      1.0,  0.0,  0.0,       0.0, 0.0,
         1, -1, -1,      1.0,  0.0,  0.0,       1.0, 0.0,
         1,  1,  1,      1.0,  0.0,  0.0,       0.0, 1.0,
         1,  1, -1,      1.0,  0.0,  0.0,       1.0, 1.0,
        -1, -1, -1,     -1.0,  0.0,  0.0,       0.0, 0.0,
        -1, -1,  1,     -1.0,  0.0,  0.0,       1.0, 0.0,
        -1,  1, -1,     -1.0,  0.0,  0.0,       0.0, 1.0,
        -1,  1,  1,     -1.0,  0.0,  0.0,       1.0, 1.0
    };

static Mesh* createTexturedCube(float size = 1.0f)
{
    float a = size;

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
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), vertexCount, true);
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


R_DynamicMeshUpdate::R_DynamicMeshUpdate()
    : _font(NULL)
    , _triangleModel(NULL)
    , _cubeModel(NULL)
    , _spinDirection(-1.0f)
{    
}



void R_DynamicMeshUpdate::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");



    // Create a perspective projection matrix.
    Matrix projMatrix;
    Matrix::createPerspective(45.0f, getWidth() / (float)getHeight(), 1.0f, 100.0f, &projMatrix);

    // Create a lookat view matrix.
    Matrix viewMatrix;
    Matrix::createLookAt(Vector3(3,2,-5), Vector3::zero(), Vector3::unitY(), &viewMatrix);

    // mvp matrix
    Matrix mvp = projMatrix * viewMatrix;


    Material* material = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag");
    Texture::Sampler * sampler = Texture::Sampler::create("res/png/brick.png");
    material->getParameter("u_diffuseTexture")->setValue(sampler);
    material->getParameter("u_worldViewProjectionMatrix")->setValue(mvp);
    material->getStateBlock()->setCullFace(true);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(true);

    // Create a cube.
    Mesh* meshQuad = createTexturedCube();
    _cubeModel = Model::create(meshQuad);
    _cubeModel->setMaterial(material);
    SAFE_RELEASE(meshQuad);


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

void R_DynamicMeshUpdate::finalize()
{
    SAFE_RELEASE(_cubeModel);
    SAFE_RELEASE(_triangleModel);
    SAFE_RELEASE(_font);
}

void R_DynamicMeshUpdate::update(float elapsedTime)
{
    // Update the rotation of the triangle. The speed is 180 degrees per second.
    _worldViewProjectionMatrix.rotateZ( _spinDirection * MATH_PI * elapsedTime * 0.001f);


    VertexBuffer * vb = (VertexBuffer *)_cubeModel->getMesh()->getVertexBuffer();
    float * data = (float*)vb->lock(0, 10, false);
    memcpy(data, vertices, sizeof(float) * 24 * 8);
    vb->unLock();
}

void R_DynamicMeshUpdate::render(float elapsedTime)
{
    // draw the cube in main view
    Game::getInstance()->bindView(0);
    _cubeModel->draw();

    // draw frame rate
    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void R_DynamicMeshUpdate::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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
