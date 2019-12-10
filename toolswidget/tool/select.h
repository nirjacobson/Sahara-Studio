#ifndef SELECT_H
#define SELECT_H

#include <QVector3D>
#include <QVector4D>

#include "tool.h"
#include "scene/node/node.h"

class Select : public Tool
{
    public:
        Select(Sahara::Scene& scene);
        ~Select();

        void mouseMoved(const QVector2D& ndc) override;
        void mousePressed(const QVector2D&) override;

    private:
        QVector3D _ray;
};

#endif // SELECT_H
