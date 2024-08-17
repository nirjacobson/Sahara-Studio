#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QMessageBox>
#include <QJsonDocument>
#include <QSettings>

#include "toolswidget/toolswidget.h"
#include "scenegraphwidget/scenegraphwidget.h"
#include "nodedetailwidget/nodedetailwidget.h"
#include "nodeitemdetailwidget/camerawidget/camerawidget.h"
#include "nodeitemdetailwidget/pointlightwidget/pointlightwidget.h"
#include "nodeitemdetailwidget/modelwidget/modelwidget.h"
#include "nodeitemdetailwidget/scenewidget/scenewidget.h"
#include "widgets/scenewidget/openglscenewidget.h"
#include "widgets/scenewidget/vulkanscenewidget.h"
#include "json/json.h"
#include "application.h"
#include "commands/transformnodecommand.h"
#include "render/renderer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr, Application* application = nullptr);
        ~MainWindow();

        bool event(QEvent* event) override;

        void updateNodeFields();
        void updateCameraFields();
        void updateModelFields();
        void updatePointLightFields();
        void updateSceneFields();

        Application* app();

        Sahara::Renderer* renderer();

    private:
        Application* _app;
        Ui::MainWindow *ui;
        bool _vulkan;
        Sahara::SceneWidget* _primarySceneWidget;
        ToolsWidget* _toolsWidget;
        SceneGraphWidget* _sceneGraphWidget;
        NodeDetailWidget* _nodeDetailWidget;
        QScrollArea _nodeDetailWidgetScrollArea;
        CameraWidget* _cameraWidget;
        QScrollArea _cameraWidgetScrollArea;
        PointLightWidget* _pointLightWidget;
        QScrollArea _pointLightWidgetScrollArea;
        ModelWidget* _modelWidget;
        SceneWidget* _sceneWidget;
        QScrollArea _sceneWidgetScrollArea;
        Sahara::Renderer* _renderer;

        Sahara::Node* _selectedNode;

    private slots:
        void sceneWidgetInitialized();
        void sceneWidgetSizeChanged(QSize);
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
        void openGLTriggered();
        void vulkanTriggered();
};

#endif // MAINWINDOW_H
