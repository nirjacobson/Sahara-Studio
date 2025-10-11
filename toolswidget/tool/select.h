#ifndef SELECT_H
#define SELECT_H

#include <QVector3D>
#include <QVector4D>

#include "tool.h"
#include <Sahara/scene/model.h>
#include <Sahara/scene/asset/mesh/surface.h>
#include <Sahara/scene/node/node.h>
#include <Sahara/scene/asset/mesh/surface.h>
#include <Sahara/scene/instance/instancemesh.h>
#include <Sahara/scene/instance/instancecontroller.h>

class Select : public Tool
{
    Q_OBJECT

    public:
        enum Mode {
            OBJECTS,
            SURFACES
        };

        Select(Sahara::Scene& scene);
        ~Select();

        void mouseMoved(const QVector2D& ndc) override;
        void mousePressed(const QVector2D&) override;

        void setMode(const Mode mode);
        void select(Sahara::Node* model);
        void select(Sahara::Model* model, Sahara::Instance* instance, const int surfaceIdx);

    signals:
        void updatedScene();
        void selectedSurface(Sahara::Model* model, Sahara::Instance* instance, const int surfaceIdx);

    private:
        QVector3D _ray;
        Mode _mode;
        Sahara::Node* _clickedModel;
        Sahara::Instance* _clickedInstance;

        void mousePressedObjects();
        void mousePressedSurfaces();
};

#endif // SELECT_H
