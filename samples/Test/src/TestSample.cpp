#include "TestSample.h"

// Declare our game instance
TestSample game;


/*
 *
 *  Default culling is clock wize order, so draw in counter clockwize.
 *
 *  p1 p6   *-----*  p2        0,1 *-----*  1,1
 *          | \   |                | \   |
 *          |  \  |                |  \  |
 *          |   \ |                |   \ |
 *          |    \|                |    \|
 *      p5  *-----*  p3 p4     0,0 *-----*  1,0
 *
 * */

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

static Mesh* createStaticMesh()
{
    Vector2 p1(-1, 1);
    Vector2 p2( 1, 1);
    Vector2 p3( 1,-1);

    Vector2 p4( 1,-1);
    Vector2 p5(-1,-1);
    Vector2 p6(-1, 1);

    float vertices[] =
    {
        p1.x, p1.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
        p2.x, p2.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,1,
        p3.x, p3.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,

        p4.x, p4.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,
        p5.x, p5.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,0,
        p6.x, p6.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
    };
    unsigned int vertexCount = 6;
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

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

static Mesh* createStaticIndexedMesh()
{
    Vector2 p1(-1, 1);
    Vector2 p2( 1, 1);
    Vector2 p3( 1,-1);
    Vector2 p5(-1,-1);

    float vertices[] =
    {
        p1.x, p1.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
        p2.x, p2.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,1,
        p3.x, p3.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,
        p5.x, p5.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,0,
    };
    unsigned int vertexCount = 4;
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


    // indices

    short indices[] =
    {
        0,3,2,
        0,2,1
    };

    MeshPart * part = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, 6, false);
    part->setIndexData(indices, 0, 6);

    return mesh;
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------


static Mesh* createDynamicMesh()
{
    Vector2 p1(-1, 1);
    Vector2 p2( 1, 1);
    Vector2 p3( 1,-1);

    Vector2 p4( 1,-1);
    Vector2 p5(-1,-1);
    Vector2 p6(-1, 1);

    /*float vertices[] =
    {
        p1.x, p1.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
        p2.x, p2.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,1,
        p3.x, p3.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,

        p4.x, p4.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,
        p5.x, p5.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,0,
        p6.x, p6.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
    };*/
    unsigned int vertexCount = 6;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 4),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), vertexCount, true);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::TRIANGLES);
    //mesh->setVertexData(vertices, 0, vertexCount);

    return mesh;
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------


static Mesh* createDynamicIndexedMesh()
{
    Vector2 p1(-1, 1);
    Vector2 p2( 1, 1);
    Vector2 p3( 1,-1);
    Vector2 p5(-1,-1);

    /*float vertices[] =
    {
        p1.x, p1.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
        p2.x, p2.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,1,
        p3.x, p3.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,
        p5.x, p5.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,0,
    };*/
    unsigned int vertexCount = 4;
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 4),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 3), vertexCount, true);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::TRIANGLES);
    //mesh->setVertexData(vertices, 0, vertexCount);


    // indices

    short indices[] =
    {
        0,3,2,
        0,2,1
    };

    MeshPart * part = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, 6, true);
    //part->setIndexData(indices, 0, 6);

    return mesh;
}










//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

Mesh * _mesh;
MeshBatch * _meshBatch;

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
    //Mesh* mesh = createStaticMesh();
    //Mesh* mesh = createStaticIndexedMesh();
    //Mesh * mesh = createDynamicMesh();
    Mesh * mesh = createDynamicIndexedMesh();

    _mesh = mesh;


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
    Texture::Sampler* sampler = material->getParameter("s_texColor")->setValue("res/png/brick.png", mipmap);
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



    // MeshBatch

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 4),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    _meshBatch = MeshBatch::create(VertexFormat(elements, 3), Mesh::PrimitiveType::TRIANGLES, material, true);




    /*Vector2 p1(-1, 1);
    Vector2 p2( 1, 1);
    Vector2 p3( 1,-1);

    Vector2 p4( 1,-1);
    Vector2 p5(-1,-1);
    Vector2 p6(-1, 1);

    float vertices[] =
    {
        p1.x, p1.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
        p2.x, p2.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,1,
        p3.x, p3.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,

        p4.x, p4.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,
        p5.x, p5.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,0,
        p6.x, p6.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
    };
    unsigned int vertexCount = 6;


    _meshBatch->start();
    _meshBatch->add(vertices, vertexCount);
    _meshBatch->finish();*/

}

void TestSample::finalize()
{
    // Model and font are reference counted and should be released before closing this sample.
    SAFE_RELEASE(_model);
}

float tt = 0.0f;

void TestSample::update(float elapsedTime)
{
    // Update the rotation of the triangle. The speed is 180 degrees per second.
    _worldViewProjectionMatrix.rotateZ( _spinDirection * MATH_PI * elapsedTime * 0.001f);


    tt += elapsedTime * 0.001f;

    float dx = sin(tt) * 0.1;
    float dy = cos(tt) * 0.1;


// for dynamic mesh
#if 0
    Vector2 p1(-1 + dx, 1 + dy);
    Vector2 p2( 1 - dx, 1 + dy);
    Vector2 p3( 1 + dx,-1 - dy);

    Vector2 p4( 1 - dx,-1 + dy);
    Vector2 p5(-1 - dx,-1 - dy);
    Vector2 p6(-1 - dx, 1 + dy);

    float vertices[] =
    {
        p1.x, p1.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
        p2.x, p2.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,1,
        p3.x, p3.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,

        p4.x, p4.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,
        p5.x, p5.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,0,
        p6.x, p6.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
    };

    _mesh->setVertexData(vertices, 0, 6);
#endif

// for dynamic index mesh
#if 0
    Vector2 p1(-1 + dx, 1 + dy);
    Vector2 p2( 1 - dx, 1 + dy);
    Vector2 p3( 1 + dx,-1 - dy);
    Vector2 p5(-1 - dx,-1 - dy);

    float vertices[] =
    {
        p1.x, p1.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
        p2.x, p2.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,1,
        p3.x, p3.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,
        p5.x, p5.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,0,
    };

    _mesh->setVertexData(vertices, 0, 4);


    short indices[] =
    {
        0,3,2,
        0,2,1
    };

    _mesh->getPart(0)->setIndexData(indices, 0, 6);

#endif





}

void TestSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Bind the view projection matrix to the model's parameter. This will transform the vertices when the model is drawn.
    _model->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(_worldViewProjectionMatrix);
    //_model->draw();






#if 1
 // non index meshbatch
#if 0

    Vector2 p1(-1, 1);
    Vector2 p2( 1, 1);
    Vector2 p3( 1,-1);

    Vector2 p4( 1,-1);
    Vector2 p5(-1,-1);
    Vector2 p6(-1, 1);

    float vertices[] =
    {
        p1.x, p1.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
        p2.x, p2.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,1,
        p3.x, p3.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,

        p4.x, p4.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,
        p5.x, p5.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,0,
        p6.x, p6.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
    };
    unsigned int vertexCount = 6;


    _meshBatch->start();
    _meshBatch->add(vertices, vertexCount);
    _meshBatch->finish();

#else


    Vector2 p1(-1, 1);
    Vector2 p2( 1, 1);
    Vector2 p3( 1,-1);
    Vector2 p5(-1,-1);

    float vertices[] =
    {
        p1.x, p1.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,1,
        p2.x, p2.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,1,
        p3.x, p3.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        1,0,
        p5.x, p5.y, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f,        0,0,
    };
    unsigned int vertexCount = 4;

    unsigned short indices[] =
    {
        0,3,2,
        0,2,1
    };

    unsigned int indexCount = 6;


    _meshBatch->start();
    _meshBatch->add(vertices, vertexCount, indices, indexCount);
    _meshBatch->finish();

#endif

#endif




    _meshBatch->draw();
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
