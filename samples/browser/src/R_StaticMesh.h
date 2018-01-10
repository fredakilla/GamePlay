#ifndef R_STATIC_MESH_H_
#define R_STATIC_MESH_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;


class R_StaticMesh : public Sample
{
public:

    R_StaticMesh();
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

private:

    Font* _font;
    Model* _model;
    float _spinDirection;
    Matrix _worldViewProjectionMatrix;
};

#endif
