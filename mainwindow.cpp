#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _sceneGraphWidget(nullptr)
{
    ui->setupUi(this);

    connect(ui->sceneWidget, &Sahara::SceneWidget::initialized, this, &MainWindow::sceneWidgetInitialized);
    connect(ui->sceneWidget, &Sahara::SceneWidget::sizeChanged, this, &MainWindow::sceneWidgetSizeChanged);
    connect(ui->sceneWidget, &Sahara::SceneWidget::cameraMotion, this, &MainWindow::sceneWidgetCameraMotion);

    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);

    _nodeDetailWidgetScrollArea.setWidget(&_nodeDetailWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
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
}

void MainWindow::sceneWidgetCameraMotion()
{
    if (_selectedNode == &ui->sceneWidget->scene().cameraNode()) {
        _nodeDetailWidget.update();
    }
}

void MainWindow::sceneGraphWidgetSelectionChanged(Sahara::Node* node)
{
    _selectedNode = node;

    if (node) {
        _nodeDetailWidget.setNode(node);
        ui->nodeDetailDockWidget->setWidget(&_nodeDetailWidgetScrollArea);
    } else {
        ui->nodeDetailDockWidget->setWidget(nullptr);
    }
}

