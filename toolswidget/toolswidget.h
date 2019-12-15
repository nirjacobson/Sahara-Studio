#ifndef TOOLSWIDGET_H
#define TOOLSWIDGET_H

#include <QWidget>

#include "scene/scene.h"
#include "tool/select.h"
#include "tool/move.h"

namespace Ui {
class ToolsWidget;
}

class ToolsWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit ToolsWidget(Sahara::Scene& scene, QWidget *parent = nullptr);
        ~ToolsWidget();

        void setScene(Sahara::Scene& scene);

    public slots:
        void mouseMoved(const QVector2D& ndc);
        void mousePressed(const QVector2D& ndc);

    signals:
        void updatedScene();
        void updatedNode();

    private:
        Ui::ToolsWidget *ui;
        Select _select;
        Move _move;

        Tool* _tool;

        void toolButtonClicked();

    private slots:
        void selectToolButtonClicked();
        void moveToolButtonClicked();
        void toolReleased();
};

#endif // TOOLSWIDGET_H
