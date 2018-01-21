#include "R_Lights.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Renderer", "Lights", R_Lights, 5);
#endif

#include <bx/allocator.h>
    #include <bx/math.h>
#include "renderer/BGFX/BGFXVertexBuffer.h"

    void calcTangents(void* _vertices, uint16_t _numVertices, bgfx::VertexDecl _decl, const uint16_t* _indices, uint32_t _numIndices)
    {
        struct PosTexcoord
        {
            float m_x;
            float m_y;
            float m_z;
            float m_pad0;
            float m_u;
            float m_v;
            float m_pad1;
            float m_pad2;
        };

        float* tangents = new float[6*_numVertices];
        bx::memSet(tangents, 0, 6*_numVertices*sizeof(float) );

        PosTexcoord v0;
        PosTexcoord v1;
        PosTexcoord v2;

        for (uint32_t ii = 0, num = _numIndices/3; ii < num; ++ii)
        {
            const uint16_t* indices = &_indices[ii*3];
            uint32_t i0 = indices[0];
            uint32_t i1 = indices[1];
            uint32_t i2 = indices[2];

            bgfx::vertexUnpack(&v0.m_x, bgfx::Attrib::Position,  _decl, _vertices, i0);
            bgfx::vertexUnpack(&v0.m_u, bgfx::Attrib::TexCoord0, _decl, _vertices, i0);

            bgfx::vertexUnpack(&v1.m_x, bgfx::Attrib::Position,  _decl, _vertices, i1);
            bgfx::vertexUnpack(&v1.m_u, bgfx::Attrib::TexCoord0, _decl, _vertices, i1);

            bgfx::vertexUnpack(&v2.m_x, bgfx::Attrib::Position,  _decl, _vertices, i2);
            bgfx::vertexUnpack(&v2.m_u, bgfx::Attrib::TexCoord0, _decl, _vertices, i2);

            const float bax = v1.m_x - v0.m_x;
            const float bay = v1.m_y - v0.m_y;
            const float baz = v1.m_z - v0.m_z;
            const float bau = v1.m_u - v0.m_u;
            const float bav = v1.m_v - v0.m_v;

            const float cax = v2.m_x - v0.m_x;
            const float cay = v2.m_y - v0.m_y;
            const float caz = v2.m_z - v0.m_z;
            const float cau = v2.m_u - v0.m_u;
            const float cav = v2.m_v - v0.m_v;

            const float det = (bau * cav - bav * cau);
            const float invDet = 1.0f / det;

            const float tx = (bax * cav - cax * bav) * invDet;
            const float ty = (bay * cav - cay * bav) * invDet;
            const float tz = (baz * cav - caz * bav) * invDet;

            const float bx = (cax * bau - bax * cau) * invDet;
            const float by = (cay * bau - bay * cau) * invDet;
            const float bz = (caz * bau - baz * cau) * invDet;

            for (uint32_t jj = 0; jj < 3; ++jj)
            {
                float* tanu = &tangents[indices[jj]*6];
                float* tanv = &tanu[3];
                tanu[0] += tx;
                tanu[1] += ty;
                tanu[2] += tz;

                tanv[0] += bx;
                tanv[1] += by;
                tanv[2] += bz;
            }
        }

        for (uint32_t ii = 0; ii < _numVertices; ++ii)
        {
            const float* tanu = &tangents[ii*6];
            const float* tanv = &tangents[ii*6 + 3];

            float normal[4];
            bgfx::vertexUnpack(normal, bgfx::Attrib::Normal, _decl, _vertices, ii);
            float ndt = bx::vec3Dot(normal, tanu);

            float nxt[3];
            bx::vec3Cross(nxt, normal, tanu);

            float tmp[3];
            tmp[0] = tanu[0] - normal[0] * ndt;
            tmp[1] = tanu[1] - normal[1] * ndt;
            tmp[2] = tanu[2] - normal[2] * ndt;

            float tangent[4];
            bx::vec3Norm(tangent, tmp);

            tangent[3] = bx::vec3Dot(nxt, tanv) < 0.0f ? -1.0f : 1.0f;
            bgfx::vertexPack(tangent, true, bgfx::Attrib::Tangent, _decl, _vertices, ii);
        }

        delete [] tangents;
    }


#include <bx/pixelformat.h>

    inline uint32_t encodeNormalRgba8(float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f)
    {
        const float src[] =
        {
            _x * 0.5f + 0.5f,
            _y * 0.5f + 0.5f,
            _z * 0.5f + 0.5f,
            _w * 0.5f + 0.5f,
        };
        uint32_t dst;
        bx::packRgba8(&dst, src);
        return dst;
    }

static Mesh* createTexturedCube(float size = 1.0f)
{

    struct PosColorVertex
    {
        Vector3 m_pos;
        uint32_t m_normal;
        uint32_t m_tangent;
        int16_t m_u;
        int16_t m_v;
    };

    PosColorVertex vertices[] =
    {
           // position             // normal                                  // tangent        // texcoord
        {  Vector3(-1, -1,  1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x0000, 0x0000  },
        {  Vector3( 1, -1,  1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x7fff, 0x0000  },
        {  Vector3(-1,  1,  1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x0000, 0x7fff  },
        {  Vector3( 1,  1,  1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x7fff, 0x7fff  },
        {  Vector3(-1,  1,  1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x0000, 0x0000  },
        {  Vector3( 1,  1,  1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x7fff, 0x0000  },
        {  Vector3(-1,  1, -1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x0000, 0x7fff  },
        {  Vector3( 1,  1, -1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x7fff, 0x7fff  },
        {  Vector3(-1,  1, -1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x0000, 0x0000  },
        {  Vector3( 1,  1, -1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x7fff, 0x0000  },
        {  Vector3(-1, -1, -1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x0000, 0x7fff  },
        {  Vector3( 1, -1, -1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x7fff, 0x7fff  },
        {  Vector3(-1, -1, -1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x0000, 0x0000  },
        {  Vector3( 1, -1, -1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x7fff, 0x0000  },
        {  Vector3(-1, -1,  1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x0000, 0x7fff  },
        {  Vector3( 1, -1,  1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x7fff, 0x7fff  },
        {  Vector3( 1, -1,  1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x0000, 0x0000  },
        {  Vector3( 1, -1, -1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x7fff, 0x0000  },
        {  Vector3( 1,  1,  1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x0000, 0x7fff  },
        {  Vector3( 1,  1, -1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x7fff, 0x7fff  },
        {  Vector3(-1, -1, -1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x0000, 0x0000  },
        {  Vector3(-1, -1,  1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x7fff, 0x0000  },
        {  Vector3(-1,  1, -1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x0000, 0x7fff  },
        {  Vector3(-1,  1,  1),    encodeNormalRgba8( 0.0f,  0.0f,  1.0f),    0,                0x7fff, 0x7fff  }
    };

    unsigned short indices[] =
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
        VertexFormat::Element(VertexFormat::POSITION,   3),
        VertexFormat::Element(VertexFormat::NORMAL,     4, VertexFormat::Uint8, true),
        VertexFormat::Element(VertexFormat::TANGENT,    4, VertexFormat::Uint8, true),
        VertexFormat::Element(VertexFormat::TEXCOORD0,  2, VertexFormat::Int16, true)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, BX_COUNTOF(elements)), vertexCount, true);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setVertexData(vertices, 0, vertexCount);


    BGFXVertexBuffer * vb = (BGFXVertexBuffer*)mesh->getVertexBuffer();

    calcTangents(vertices
                    , vertexCount
                    , vb->getVertexDecl()// PosNormalTangentTexcoordVertex::ms_decl
                    , indices
                    , indexCount
    );


    MeshPart* meshPart = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, indexCount, true);
    meshPart->setIndexData(indices, 0, indexCount);
    return mesh;
}


R_Lights::R_Lights()
    : _font(NULL)
    , _triangleModel(NULL)
    , _cubeModel(NULL)
    , _spinDirection(-1.0f)
{    
}



void R_Lights::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");



    // Create a perspective projection matrix.
    Matrix projMatrix;
    Matrix::createPerspective(45.0f, getWidth() / (float)getHeight(), 1.0f, 1000.0f, &projMatrix);

    // Create a lookat view matrix.
    Matrix viewMatrix;
    Matrix::createLookAt(Vector3(3,2,-5), Vector3::zero(), Vector3::unitY(), &viewMatrix);

    // set mvp matrix
    _worldViewProjectionMatrix = projMatrix * viewMatrix;
    _worldViewProjectionMatrix.scale(1.0f);


    // create  material
    Material* material = Material::create("res/shaders/light.vert", "res/shaders/light.frag");

    Texture::Sampler * sampler1 = Texture::Sampler::create("res/png/brick.png");
    material->getParameter("s_texColor")->setValue(sampler1);

    Texture::Sampler * sampler2 = Texture::Sampler::create("res/png/brickn.png");
    material->getParameter("s_texNormal")->setValue(sampler2);

    material->getParameter("u_worldViewProjectionMatrix")->setValue(_worldViewProjectionMatrix);

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

void R_Lights::finalize()
{
    SAFE_RELEASE(_cubeModel);
    SAFE_RELEASE(_triangleModel);
    SAFE_RELEASE(_font);
}

void R_Lights::update(float elapsedTime)
{
    // Update the rotation of the cube.
    float dt = elapsedTime * 0.001f;
    float dx = _spinDirection * MATH_PI * 1.5f;
    float dy = _spinDirection * MATH_PI * 1.2f;
    float dz = _spinDirection * MATH_PI * 1.3f;
    Quaternion rot = Quaternion(Vector3(dx, dy, dz), dt);
    //_worldViewProjectionMatrix.rotate(rot);
}

void R_Lights::render(float elapsedTime)
{
    // draw the cube in main view
    Game::getInstance()->bindView(0);
    _cubeModel->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(_worldViewProjectionMatrix);




    int m_numLights = 4;

    float time = Game::getAbsoluteTime() * 0.001f;

    Vector4 lightPosRadius[4];
    for (uint32_t ii = 0; ii < m_numLights; ++ii)
    {
        lightPosRadius[ii].x = sin( (time*(0.1f + ii*0.17f) + ii*bx::kPiHalf*1.37f ) )*3.0f;
        lightPosRadius[ii].y = cos( (time*(0.2f + ii*0.29f) + ii*bx::kPiHalf*1.49f ) )*3.0f;
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

    _cubeModel->getMaterial()->getParameter("u_lightPosRadius")->setValue(lightPosRadius, m_numLights);
    _cubeModel->getMaterial()->getParameter("u_lightRgbInnerR")->setValue(lightRgbInnerR, m_numLights);







    _cubeModel->draw();

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
