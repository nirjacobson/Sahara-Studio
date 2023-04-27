#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>

#include "scene/scene.h"

namespace Ui {
class SceneWidget;
}

class SceneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SceneWidget(QWidget *parent = nullptr);
    ~SceneWidget();

    void setScene(Sahara::Scene* scene);

private:
    Ui::SceneWidget *ui;

    Sahara::Scene* _scene;

    void populateFieldsFromScene();

private slots:
    void fieldValueChanged();
};

#endif // SCENEWIDGET_H
