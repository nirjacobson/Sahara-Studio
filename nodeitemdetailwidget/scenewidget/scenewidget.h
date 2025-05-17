#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>

#include <Sahara/scene/scene.h>
#include "commands/updatesceneambientlightcommand.h"

class MainWindow;

namespace Ui {
class SceneWidget;
}

class SceneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SceneWidget(MainWindow* window, QWidget *parent = nullptr);
    ~SceneWidget();

    void setScene(Sahara::Scene* scene);

    void updateFields();

private:
    MainWindow* _window;
    Ui::SceneWidget *ui;

    Sahara::Scene* _scene;

    void populateFieldsFromScene();

private slots:
    void ambientColorChanged(const QColor& color);
    void ambientStrengthChanged(double value);
};

#endif // SCENEWIDGET_H
