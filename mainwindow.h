#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>

#include "widgets/scenewidget/scenewidget.h"
#include "scenegraphwidget/scenegraphwidget.h"
#include "nodedetailwidget/nodedetailwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;
        SceneGraphWidget* _sceneGraphWidget;
        NodeDetailWidget _nodeDetailWidget;
        QScrollArea _nodeDetailWidgetScrollArea;

        Sahara::Node* _selectedNode;

    private slots:
        void sceneWidgetInitialized();
        void sceneWidgetSizeChanged(QSize size);
        void sceneWidgetCameraMotion();
        void sceneGraphWidgetSelectionChanged(Sahara::Node* node);
};

#endif // MAINWINDOW_H
