#ifndef MOVE_H
#define MOVE_H

#include <QVector3D>

#include "tool.h"

class Move : public Tool
{
    public:
        Move(Sahara::Scene& scene);
        ~Move();

        void mouseMoved(const QVector2D& ndc) override;
        void mousePressed(const QVector2D&) override;
};

#endif // MOVE_H
