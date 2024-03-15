#include "move.h"

Move::Move(Sahara::Scene& scene)
    : Tool(scene)
{

}

Move::~Move()
{

}

const QMatrix4x4& Move::transform() const
{
    return _transform;
}

void Move::mouseMoved(const QVector2D& ndc)
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

    QVector3D l = QVector3D(ray).normalized();

    Sahara::Node* focusNode = scene().focusNode();
    if (focusNode) {
        QVector3D p0 = scene().cameraNode().globalTransform().inverted().map(focusNode->globalPosition());
        QVector3D l0;
        QVector3D n = {0, 0, 1};

        float t = QVector3D::dotProduct(p0 - l0, n) / QVector3D::dotProduct(l, n);

        QVector3D p = l0 + l * t;
        QVector3D globalPosition = scene().cameraNode().globalTransform().map(p);

        QMatrix4x4 newTransform = focusNode->transform();
        if (!focusNode->isRoot()) {
            QVector3D newPositionLocalSpace = focusNode->parent().globalTransform().inverted().map(globalPosition);
            newTransform.setColumn(3, {newPositionLocalSpace, 1});
        } else {
            newTransform.setColumn(3, {globalPosition, 1});
        }
        _transform = newTransform;

        emit mouseMotion();
    }
}

void Move::mousePressed(const QVector2D&)
{
    emit released();
}
