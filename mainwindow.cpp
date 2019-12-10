#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _sceneGraphWidget(nullptr)
{
    ui->setupUi(this);

    _nodeDetailWidget = new NodeDetailWidget;
    _cameraWidget = new CameraWidget;

    connect(ui->sceneWidget, &Sahara::SceneWidget::initialized, this, &MainWindow::sceneWidgetInitialized);
    connect(ui->sceneWidget, &Sahara::SceneWidget::sizeChanged, this, &MainWindow::sceneWidgetSizeChanged);
    connect(ui->sceneWidget, &Sahara::SceneWidget::cameraMotion, this, &MainWindow::sceneWidgetCameraMotion);

    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);

    _nodeDetailWidgetScrollArea.setWidget(_nodeDetailWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _cameraWidget;
    delete _nodeDetailWidget;
}

void MainWindow::sceneWidgetInitialized()
{
    ui->sceneWidget->showGrid(true);

    _sceneGraphWidget = new SceneGraphWidget(*ui->sceneWidget);

    connect(_sceneGraphWidget, &SceneGraphWidget::selectionChanged, this, &MainWindow::sceneGraphWidgetSelectionChanged);

    ui->sceneGraphDockWidget->setWidget(_sceneGraphWidget);
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
        _nodeDetailWidget->setNode(node);
        ui->nodeDetailDockWidget->setWidget(&_nodeDetailWidgetScrollArea);

        Sahara::Camera* camera;
        if ((camera = dynamic_cast<Sahara::Camera*>(&node->item()))) {
            _cameraWidget->setCamera(camera);
            _nodeItemDetailWidgetScrollArea.setWidget(_cameraWidget);
            ui->nodeItemDetailDockWidget->setWidget(&_nodeItemDetailWidgetScrollArea);
        }
    } else {
        ui->nodeDetailDockWidget->setWidget(nullptr);
        ui->nodeItemDetailDockWidget->setWidget(nullptr);
    }
}

