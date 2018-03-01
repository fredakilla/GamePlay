#ifndef R_DYNAMIC_MESH_UPDATE_H_
#define R_DYNAMIC_MESH_UPDATE_H_

#include "gameplay.h"
#include "Example.h"

using namespace gameplay;


class DynamicMeshUpdate : public Example
{
public:

    DynamicMeshUpdate();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

private:

    Font* _font;
    Model* _triangleModel;
    float _spinDirection;
    Matrix _worldViewProjectionMatrix;
    Model* _cubeModel;
};

#endif
