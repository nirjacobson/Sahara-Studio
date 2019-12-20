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
#include "nodeitemdetailwidget/modelwidget/modelwidget.h"
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

        bool event(QEvent* event) override;

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
        ModelWidget* _modelWidget;

        Sahara::Node* _selectedNode;

    private slots:
        void sceneWidgetInitialized();
        void sceneWidgetSizeChanged(QSize size);
        void sceneWidgetCameraMotion();
        void sceneGraphWidgetSelectionChanged(Sahara::Node* node);
        void newActionTriggered();
        void openActionTriggered();
        void saveAsActionTriggered();
        void sceneLoaded();
        void viewMenuAboutToShow();
        void fpsActionTriggered();
        void gridActionTriggered();
        void axesActionTriggered();
        void lightsActionTriggered();
        void camerasActionTriggered();
};

#endif // MAINWINDOW_H
