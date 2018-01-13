#include "../core/Base.h"
#include "../graphics/Drawable.h"
#include "../graphics/Node.h"


namespace gameplay
{

Drawable::Drawable()
    : _node(NULL)
{
}

Drawable::~Drawable()
{
}

Node* Drawable::getNode() const
{
    return _node;
}

void Drawable::setNode(Node* node)
{
    _node = node;
}

}
