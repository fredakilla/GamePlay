#include "R_StaticMesh.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Renderer", "StaticMesh", R_StaticMesh, 0);
#endif



static Mesh* createStaticCube()
{
    struct PosColorVertex
    {
        Vector3 m_pos;
        Vector4 m_color;
    };

    float side = 1.0f;

    static PosColorVertex s_cubeVertices[] =
    {
        { Vector3(-side,  side,  side),   Vector4(1.0, 0.0, 0.0, 1.0 ) },
        { Vector3( side,  side,  side),   Vector4(0.0, 1.0, 0.0, 1.0 ) },
        { Vector3(-side, -side,  side),   Vector4(0.0, 0.0, 1.0, 1.0 ) },
        { Vector3( side, -side,  side),   Vector4(1.0, 1.0, 0.0, 1.0 ) },
        { Vector3(-side,  side, -side),   Vector4(1.0, 0.0, 1.0, 1.0 ) },
        { Vector3( side,  side, -side),   Vector4(0.0, 1.0, 1.0, 1.0 ) },
        { Vector3(-side, -side, -side),   Vector4(1.0, 1.0, 0.0, 1.0 ) },
        { Vector3( side, -side, -side),   Vector4(0.0, 1.0, 0.0, 1.0 ) },
    };



    PosColorVertex s_cubeVertices2[] =
    {
        {  Vector3(-side, -side, -side),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        {  Vector3( side, -side, -side),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        {  Vector3( side,  side, -side),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        {  Vector3( side,  side, -side),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        {  Vector3(-side,  side, -side),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        {  Vector3(-side, -side, -side),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },

        {  Vector3(-side, -side,  side),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        {  Vector3( side, -side,  side),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        {  Vector3( side,  side,  side),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        {  Vector3( side,  side,  side),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        {  Vector3(-side,  side,  side),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        {  Vector3(-side, -side,  side),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },

        {  Vector3(-side,  side,  side), -Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        {  Vector3(-side,  side, -side), -Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        {  Vector3(-side, -side, -side), -Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        {  Vector3(-side, -side, -side), -Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        {  Vector3(-side, -side,  side), -Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        {  Vector3(-side,  side,  side), -Vector4(1.0f, 0.0f, 0.0f, 1.0f) },

        {  Vector3( side,  side,  side),  Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        {  Vector3( side,  side, -side),  Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        {  Vector3( side, -side, -side),  Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        {  Vector3( side, -side, -side),  Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        {  Vector3( side, -side,  side),  Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        {  Vector3( side,  side,  side),  Vector4(1.0f, 0.0f, 0.0f, 1.0f) },

        {  Vector3(-side, -side, -side),  Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        {  Vector3( side, -side, -side),  Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        {  Vector3( side, -side,  side),  Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        {  Vector3( side, -side,  side),  Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        {  Vector3(-side, -side,  side),  Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        {  Vector3(-side, -side, -side),  Vector4(0.0f, 1.0f, 0.0f, 1.0f) },

        {  Vector3(-side,  side, -side),  Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        {  Vector3( side,  side, -side),  Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        {  Vector3( side,  side,  side),  Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        {  Vector3( side,  side,  side),  Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        {  Vector3(-side,  side,  side),  Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        {  Vector3(-side,  side, -side),  Vector4(0.0f, 1.0f, 0.0f, 1.0f) }
    };




    static const unsigned short s_cubeTriList[] =
    {
        0, 1, 2, // 0
        1, 3, 2,
        4, 6, 5, // 2
        5, 6, 7,
        0, 2, 4, // 4
        4, 2, 6,
        1, 5, 3, // 6
        5, 7, 3,
        0, 4, 1, // 8
        4, 5, 1,
        2, 3, 6, // 10
        6, 3, 7,
    };

    // Vertex format
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 4)
    };

    // Create mesh.
    unsigned int vertexCount = 8;
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }

    // Set vertices.
    mesh->setPrimitiveType(Mesh::TRIANGLES);
    mesh->setVertexData(s_cubeVertices2, 0, 36);

    // Set indices.
    /*unsigned int indexCount = 3 * 12;
    MeshPart * part = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, indexCount, false);
    part->setIndexData(s_cubeTriList, 0, indexCount);*/

    return mesh;
}



R_StaticMesh::R_StaticMesh()
    : _font(NULL), _model(NULL), _spinDirection(-1.0f)
{
    
}

void R_StaticMesh::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

    // Create an orthographic projection matrix.
    float ratio = getWidth() / (float)getHeight();
    //Matrix::createOrthographic(width, height, -1.0f, 1.0f, &_worldViewProjectionMatrix);
    Matrix::createPerspective(45.0f, ratio, 1.0f, 1000.0f, &_worldViewProjectionMatrix);

    _worldViewProjectionMatrix.translate(Vector3(0,0,-15.0f));

    // Create the triangle mesh.
    Mesh* mesh = createStaticCube();

    // Create a model for the triangle mesh. A model is an instance of a Mesh that can be drawn with a specified material.
    _model = Model::create(mesh);
    SAFE_RELEASE(mesh);

    // Create a material from the built-in "colored-unlit" vertex and fragment shaders.
    Material * material = _model->setMaterial("res/bgfxshaders/Colored_VS.bin", "res/bgfxshaders/Colored_VERTEX_COLOR_FS.bin", "VERTEX_COLOR");
    material->getStateBlock()->setCullFace(false);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(true);

    // Set default view.
    Game * game = Game::getInstance();
    View defaultView;
    defaultView.id = 0;
    defaultView.clearColor = 0x111111ff;
    defaultView.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
    defaultView.depth = 1.0f;
    defaultView.rectangle = Rectangle(game->getWidth(), game->getHeight());
    game->addView(defaultView);
}

void R_StaticMesh::finalize()
{
    // Model and font are reference counted and should be released before closing this sample.
    SAFE_RELEASE(_model);
    SAFE_RELEASE(_font);
}

void R_StaticMesh::update(float elapsedTime)
{
    // Update the rotation of the triangle. The speed is 180 degrees per second.
    //_worldViewProjectionMatrix.rotateZ( _spinDirection * MATH_PI * elapsedTime * 0.001f);
    _worldViewProjectionMatrix.rotate(Vector3(1,2,3), _spinDirection * MATH_PI * elapsedTime * 0.001f);
}

void R_StaticMesh::render(float elapsedTime)
{
    Game::getInstance()->bindView(0);

    _model->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(_worldViewProjectionMatrix);
    _model->draw();

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void R_StaticMesh::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (x < 75 && y < 50)
        {
            // Toggle Vsync if the user touches the top left corner
            setVsync(!isVsync());
        }
        else
        {
            // Reverse the spin direction if the user touches the screen.
            _spinDirection *= -1.0f;
        }
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}
