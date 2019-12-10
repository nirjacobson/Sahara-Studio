#include "select.h"

Select::Select(Sahara::Scene& scene)
    : Tool(scene)
{

}

Select::~Select()
{

}

void Select::mouseMoved(const QVector2D& ndc)
{
    QVector4D ray = {
        ndc.x(),
        ndc.y(),
        -1,
        1
    };

    ray = scene().camera().projection().inverted().map(ray);
    ray[2] = -1;
    ray[3] = 0;

    ray = scene().cameraNode().globalTransform().map(ray);

    _ray = QVector3D(ray).normalized();
}

void Select::mousePressed(const QVector2D&)
{
    QVector3D rayPoint = scene().cameraNode().globalPosition();
    QVector3D raySegment = _ray * 0.2f;
    Sahara::Node* pickedNode = nullptr;

    scene().root().depthFirst([](Sahara::Node&, auto){}, [&](Sahara::Node& node, auto stop) {
        if (&node != &scene().cameraNode()) {
            rayPoint = scene().cameraNode().globalPosition();
            for (int i = 0; i < 1000; i++) {
                if (node.intersects(rayPoint)) {
                    pickedNode = &node;
                    stop();
                    break;
                }

                rayPoint += raySegment;
            }
        }
    });

    if (pickedNode)
        pickedNode->setFocus(true);
    else
        scene().root().setFocus(true);

    emit updatedScene();
}
