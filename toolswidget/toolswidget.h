#ifndef TOOLSWIDGET_H
#define TOOLSWIDGET_H

#include <QWidget>

#include <Sahara/scene/scene.h>
#include "tool/select.h"
#include "tool/move.h"
#include "commands/transformnodecommand.h"

class MainWindow;

namespace Ui {
class ToolsWidget;
}

class ToolsWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit ToolsWidget(MainWindow* window, Sahara::Scene& scene, QWidget *parent = nullptr);
        ~ToolsWidget();

        void setScene(Sahara::Scene& scene);

    public slots:
        void mouseMoved(const QVector2D& ndc);
        void mousePressed(const QVector2D& ndc);

    signals:
        void updatedScene();

    private:
        MainWindow* _window;
        Sahara::Scene& _scene;
        Ui::ToolsWidget *ui;
        Select _select;
        Move _move;

        Tool* _tool;

        void toolButtonClicked();

    private slots:
        void selectToolButtonClicked();
        void moveToolButtonClicked();
        void toolReleased();
        void moveMotion();
};

#endif // TOOLSWIDGET_H
