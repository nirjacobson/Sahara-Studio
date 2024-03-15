#ifndef MOVE_H
#define MOVE_H

#include <QVector3D>

#include "tool.h"

class Move : public Tool
{
    Q_OBJECT

    public:
        Move(Sahara::Scene& scene);
        ~Move();

        const QMatrix4x4& transform() const;

        void mouseMoved(const QVector2D& ndc) override;
        void mousePressed(const QVector2D&) override;

    signals:
        void mouseMotion();

    private:
        QMatrix4x4 _transform;
};

#endif // MOVE_H
