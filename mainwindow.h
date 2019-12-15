#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>

#include "toolswidget/toolswidget.h"
#include "widgets/scenewidget/scenewidget.h"
#include "scenegraphwidget/scenegraphwidget.h"
#include "nodedetailwidget/nodedetailwidget.h"
#include "nodeitemdetailwidget/camerawidget/camerawidget.h"
#include "nodeitemdetailwidget/pointlightwidget/pointlightwidget.h"
#include "json/json.h"

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
        ToolsWidget* _toolsWidget;
        SceneGraphWidget* _sceneGraphWidget;
        NodeDetailWidget* _nodeDetailWidget;
        QScrollArea _nodeDetailWidgetScrollArea;
        CameraWidget* _cameraWidget;
        QScrollArea _cameraWidgetScrollArea;
        PointLightWidget* _pointLightWidget;
        QScrollArea _pointLightWidgetScrollArea;

        Sahara::Node* _selectedNode;

    private slots:
        void sceneWidgetInitialized();
        void sceneWidgetSizeChanged(QSize size);
        void sceneWidgetCameraMotion();
        void sceneGraphWidgetSelectionChanged(Sahara::Node* node);
        void newActionTriggered();
        void openActionTriggered();
        void saveAsActionTriggered();
};

#endif // MAINWINDOW_H
