#include "tool.h"

Tool::Tool(Sahara::Scene& scene, QObject* parent)
    : QObject(parent)
    , _scene(scene)
{

}

Tool::~Tool()
{

}

Sahara::Scene&Tool::scene()
{
    return _scene;
}
