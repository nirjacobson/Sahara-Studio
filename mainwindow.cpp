#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _toolsWidget(nullptr),
    _sceneGraphWidget(nullptr),
    _nodeDetailWidget(nullptr),
    _cameraWidget(nullptr),
    _pointLightWidget(nullptr)
{
    ui->setupUi(this);
    ui->nodeDockWidget->setWidget(nullptr);

    _nodeDetailWidget = new NodeDetailWidget;
    _cameraWidget = new CameraWidget;
    _pointLightWidget = new PointLightWidget;

    _nodeDetailWidgetScrollArea.setWidget(_nodeDetailWidget);
    _cameraWidgetScrollArea.setWidget(_cameraWidget);
    _pointLightWidgetScrollArea.setWidget(_pointLightWidget);

    connect(ui->sceneWidget, &Sahara::SceneWidget::initialized, this, &MainWindow::sceneWidgetInitialized);
    connect(ui->sceneWidget, &Sahara::SceneWidget::sizeChanged, this, &MainWindow::sceneWidgetSizeChanged);
    connect(ui->sceneWidget, &Sahara::SceneWidget::cameraMotion, this, &MainWindow::sceneWidgetCameraMotion);

    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _sceneGraphWidget;
}

void MainWindow::sceneWidgetInitialized()
{
    ui->sceneWidget->showGrid(true);

    _sceneGraphWidget = new SceneGraphWidget(*ui->sceneWidget);
    connect(_sceneGraphWidget, &SceneGraphWidget::selectionChanged, this, &MainWindow::sceneGraphWidgetSelectionChanged);
    ui->sceneGraphDockWidget->setWidget(_sceneGraphWidget);

    _toolsWidget = new ToolsWidget(ui->sceneWidget->scene());
    connect(_toolsWidget, &ToolsWidget::updatedNode, _nodeDetailWidget, &NodeDetailWidget::updateFields);
    connect(_toolsWidget, &ToolsWidget::updatedScene, _sceneGraphWidget, &SceneGraphWidget::sceneUpdated);
    connect(ui->sceneWidget, &Sahara::SceneWidget::mouseMoved, _toolsWidget, &ToolsWidget::mouseMoved);
    connect(ui->sceneWidget, &Sahara::SceneWidget::mousePressed, _toolsWidget, &ToolsWidget::mousePressed);
    ui->toolsDockWidget->setWidget(_toolsWidget);
}

void MainWindow::sceneWidgetSizeChanged(QSize size)
{
    ui->sceneWidget->scene().camera().setAspect(static_cast<float>(size.width()) / size.height());
    if (_selectedNode == &ui->sceneWidget->scene().cameraNode()) {
        _cameraWidget->updateFields();
    }
}

void MainWindow::sceneWidgetCameraMotion()
{
    if (_selectedNode == &ui->sceneWidget->scene().cameraNode()) {
        _nodeDetailWidget->updateFields();
    }
}

void MainWindow::sceneGraphWidgetSelectionChanged(Sahara::Node* node)
{
    _selectedNode = node;

   ui->nodeTabWidget->clear();

    if (node) {
        _nodeDetailWidget->setNode(node);
        ui->nodeTabWidget->insertTab(0, &_nodeDetailWidgetScrollArea, "Node");

        Sahara::Camera* camera;
        Sahara::PointLight* pointLight;
        if ((camera = dynamic_cast<Sahara::Camera*>(&node->item()))) {
            _cameraWidget->setCamera(camera);
            ui->nodeTabWidget->insertTab(1, &_cameraWidgetScrollArea, "Camera");
        } else if ((pointLight = dynamic_cast<Sahara::PointLight*>(&node->item()))) {
            _pointLightWidget->setPointLight(pointLight);
            ui->nodeTabWidget->insertTab(1, &_pointLightWidgetScrollArea, "Point Light");
        }
        ui->nodeDockWidget->setWidget(ui->nodeTabWidget);
    } else {
        ui->nodeDockWidget->setWidget(nullptr);
    }
}

