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
    connect(ui->sceneWidget, &Sahara::SceneWidget::mouseMoved, _toolsWidget, &ToolsWidget::mouseMoved);
    connect(ui->sceneWidget, &Sahara::SceneWidget::mousePressed, _toolsWidget, &ToolsWidget::mousePressed);
    connect(_toolsWidget, &ToolsWidget::updatedScene, _sceneGraphWidget, &SceneGraphWidget::sceneUpdated);
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

    if (node) {
        _nodeDetailWidget = new NodeDetailWidget;
        connect(_toolsWidget, &ToolsWidget::updatedNode, _nodeDetailWidget, &NodeDetailWidget::updateFields);
        _nodeDetailWidget->setNode(node);
        _nodeDetailWidgetScrollArea.setWidget(_nodeDetailWidget);
        ui->nodeDetailDockWidget->setWidget(&_nodeDetailWidgetScrollArea);

        Sahara::Camera* camera;
        Sahara::PointLight* pointLight;
        if ((camera = dynamic_cast<Sahara::Camera*>(&node->item()))) {
            _cameraWidget = new CameraWidget;
            _cameraWidget->setCamera(camera);
            _nodeItemDetailWidgetScrollArea.setWidget(_cameraWidget);
            ui->nodeItemDetailDockWidget->setWidget(&_nodeItemDetailWidgetScrollArea);
        } else if ((pointLight = dynamic_cast<Sahara::PointLight*>(&node->item()))) {
            _pointLightWidget = new PointLightWidget;
            _pointLightWidget->setPointLight(pointLight);
            _nodeItemDetailWidgetScrollArea.setWidget(_pointLightWidget);
            ui->nodeItemDetailDockWidget->setWidget(&_nodeItemDetailWidgetScrollArea);
        } else {
            ui->nodeItemDetailDockWidget->setWidget(nullptr);
        }
    } else {
        ui->nodeDetailDockWidget->setWidget(nullptr);
        ui->nodeItemDetailDockWidget->setWidget(nullptr);
    }
}

