#include "tool.h"

Tool::Tool(Sahara::Scene& scene, QObject* parent)
    : QObject(parent)
    , _scene(&scene)
{

}

Tool::~Tool()
{

}

void Tool::setScene(Sahara::Scene& scene)
{
    _scene = &scene;
}

Sahara::Scene& Tool::scene()
{
    return *_scene;
}
