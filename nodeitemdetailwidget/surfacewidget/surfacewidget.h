#ifndef SURFACEWIDGET_H
#define SURFACEWIDGET_H

#include <QWidget>

#include <Sahara/scene/instance/instance.h>
#include <Sahara/scene/instance/instancemesh.h>
#include <Sahara/scene/instance/instancecontroller.h>
#include <Sahara/scene/model.h>

namespace Ui {
class SurfaceWidget;
}

class SurfaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SurfaceWidget(QWidget *parent = nullptr);
    ~SurfaceWidget();

    void setSurface(Sahara::Model *model, Sahara::Instance* inst, const int surfaceIdx);

private:
    Ui::SurfaceWidget *ui;

    Sahara::Model* _model;
    Sahara::Instance* _inst;
    int _surfaceIdx;

private slots:
    void materialChanged(const QString& name);
};

#endif // SURFACEWIDGET_H
