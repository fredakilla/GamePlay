#include "R_DrawStress.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Renderer", "DrawStress", R_DrawStress, 1);
#endif

static Mesh* createColoredCube(float size = 1.0f)
{
    float a = size;
    float vertices[] =
    {
        // position         // color
        -a, -a,  a,         0.0,  0.0,  1.0,  1.0f,
         a, -a,  a,         0.0,  0.0,  1.0,  1.0f,
        -a,  a,  a,         0.0,  0.0,  1.0,  1.0f,
         a,  a,  a,         0.0,  0.0,  1.0,  1.0f,
        -a,  a,  a,         0.0,  1.0,  0.0,  1.0f,
         a,  a,  a,         0.0,  1.0,  0.0,  1.0f,
        -a,  a, -a,         0.0,  1.0,  0.0,  1.0f,
         a,  a, -a,         0.0,  1.0,  0.0,  1.0f,
        -a,  a, -a,         0.0,  0.0,  1.0,  1.0f,
         a,  a, -a,         0.0,  0.0,  1.0,  1.0f,
        -a, -a, -a,         0.0,  0.0,  1.0,  1.0f,
         a, -a, -a,         0.0,  0.0,  1.0,  1.0f,
        -a, -a, -a,         0.0,  1.0,  0.0,  1.0f,
         a, -a, -a,         0.0,  1.0,  0.0,  1.0f,
        -a, -a,  a,         0.0,  1.0,  0.0,  1.0f,
         a, -a,  a,         0.0,  1.0,  0.0,  1.0f,
         a, -a,  a,         1.0,  0.0,  0.0,  1.0f,
         a, -a, -a,         1.0,  0.0,  0.0,  1.0f,
         a,  a,  a,         1.0,  0.0,  0.0,  1.0f,
         a,  a, -a,         1.0,  0.0,  0.0,  1.0f,
        -a, -a, -a,         1.0,  0.0,  0.0,  1.0f,
        -a, -a,  a,         1.0,  0.0,  0.0,  1.0f,
        -a,  a, -a,         1.0,  0.0,  0.0,  1.0f,
        -a,  a,  a,         1.0,  0.0,  0.0,  1.0f,
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
        VertexFormat::Element(VertexFormat::COLOR, 4),
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
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


R_DrawStress::R_DrawStress()
    : _font(NULL), _model(NULL)
{    
    _materialColored = nullptr;
    _materialTextured = nullptr;
    _form = nullptr;
    _rotValue = 0.0f;
    _maxDimensions = 20;
}

void R_DrawStress::setColoredGeometry()
{
    GP_ASSERT(_materialColored);

    SAFE_RELEASE(_model);

    Mesh* mesh = createColoredCube();
    _model = Model::create(mesh);
    _model->setMaterial(_materialColored);

    SAFE_RELEASE(mesh);
}

void R_DrawStress::setTexturedGeometry()
{
    GP_ASSERT(_materialTextured);

    SAFE_RELEASE(_model);

    Mesh* mesh = createTexturedCube();
    _model = Model::create(mesh);
    _model->setMaterial(_materialTextured);

    SAFE_RELEASE(mesh);
}

void R_DrawStress::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

    // Create a perspective projection matrix.
    float ratio = getWidth() / (float)getHeight();
    Matrix::createPerspective(45.0f, ratio, 1.0f, 1000.0f, &_worldViewProjectionMatrix);

    // Create a lookat matrix and multiply with projection matrix.
    Matrix dst;
    Vector3 eye(-80,80,-180);
    Vector3 targetPos(0,0,0);
    Vector3 up(0,1,0);
    Matrix::createLookAt(eye, targetPos, up, &dst);
    _worldViewProjectionMatrix = _worldViewProjectionMatrix * dst;

    // Create a material from the built-in "colored-unlit" vertex and fragment shaders.
    _materialColored = Material::create("res/shaders/colored.vert", "res/shaders/colored_vertex.frag", "VERTEX_COLOR");
    _materialColored->getStateBlock()->setCullFace(true);
    _materialColored->getStateBlock()->setDepthTest(true);
    _materialColored->getStateBlock()->setDepthWrite(true);

    // Create a material with texture
    _materialTextured = Material::create("res/shaders/textured.vert", "res/shaders/textured.frag");
    Texture::Sampler * sampler = Texture::Sampler::create("res/png/dirt.png");
    _materialTextured->getParameter("u_diffuseTexture")->setValue(sampler);
    _materialTextured->getStateBlock()->setCullFace(true);
    _materialTextured->getStateBlock()->setDepthTest(true);
    _materialTextured->getStateBlock()->setDepthWrite(true);

    // Set colored cube at init.
    setColoredGeometry();

    // Set default view.
    Game * game = Game::getInstance();
    View defaultView;
    defaultView.id = 0;
    defaultView.clearColor = 0x111111ff;
    defaultView.clearFlags = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH;
    defaultView.depth = 1.0f;
    defaultView.rectangle = Rectangle(game->getWidth(), game->getHeight());
    game->addView(defaultView);

    // initialise UI
    initializeUI();
}

void R_DrawStress::initializeUI()
{
    Theme::Style* style = Theme::getDefault()->getEmptyStyle();

    _form = Form::create("Form", style);
    _form->setLayout(Layout::LAYOUT_VERTICAL);
    _form->setWidth(200);
    _form->setAlignment(Control::Alignment::ALIGN_VCENTER_LEFT);

    Slider * slider = Slider::create("slider_maxdim");
    slider->setText("Dimensions");
    slider->setValueTextVisible(true);
    slider->setWidth(190);
    slider->setMin(1);
    slider->setMax(40);
    slider->setValue(_maxDimensions);
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    RadioButton * radio1 = RadioButton::create("radio_colored");
    radio1->setText("Colored");
    radio1->setSelected(true);
    radio1->addListener(this, Control::Listener::CLICK);

    RadioButton * radio5 = RadioButton::create("radio_textured");
    radio5->setText("Textured");
    radio5->setSelected(false);
    radio5->addListener(this, Control::Listener::CLICK);

    _form->addControl(slider);
    _form->addControl(radio1);
    _form->addControl(radio5);
}

void R_DrawStress::finalize()
{
    SAFE_RELEASE(_model);
    SAFE_RELEASE(_materialColored);
    SAFE_RELEASE(_materialTextured);
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_form);
}

void R_DrawStress::update(float elapsedTime)
{
    _rotValue += MATH_PI * elapsedTime * 0.001f;
}

void R_DrawStress::render(float elapsedTime)
{
    Game::getInstance()->bindView(0);

    if(_model)
    {
        const float step = 3.0f;
        float pos[3];
        pos[0] = -step*_maxDimensions / 2.0f;
        pos[1] = -step*_maxDimensions / 2.0f;
        pos[2] = -15.0;

        for(int x=0; x<_maxDimensions; ++x)
            for(int y=0; y<_maxDimensions; ++y)
                for(int z=0; z<_maxDimensions; ++z)
                {
                    Matrix matrix = _worldViewProjectionMatrix;

                    matrix.translate(Vector3(pos[0] + float(x)*step,
                                             pos[1] + float(y)*step,
                                             pos[2] + float(z)*step));

                    matrix.rotate(Quaternion(Vector3(1 + x*0.21f, 1 + y*0.37f, 1 + z*0.13f), _rotValue));

                    _model->getMaterial()->getParameter("u_worldViewProjectionMatrix")->setValue(matrix);
                    _model->draw();
                }
    }

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, getHeight()-20, getFrameRate());

    _form->draw();
}

void R_DrawStress::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:        
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}


void R_DrawStress::controlEvent(Control* control, EventType evt)
{
    Button* button = static_cast<Button*>(control);

    if (strcmp(button->getId(), "radio_colored") == 0)
    {
        setColoredGeometry();
    }
    else if (strcmp(button->getId(), "radio_textured") == 0)
    {
        setTexturedGeometry();
    }
    else if (strcmp(button->getId(), "slider_maxdim") == 0)
    {
        Slider* slider = static_cast<Slider*>(control);
        _maxDimensions = slider->getValue();
    }
}
